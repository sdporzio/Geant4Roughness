#include "ptfe_primaryGeneratorAction.hh"
#include "ptfe_detectorConstruction.hh"

#include "G4ParticleGun.hh"
#include "G4RunManager.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Proton.hh"
#include "G4IonTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"

#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolume.hh"


ptfe_primaryGeneratorAction::ptfe_primaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleSource(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);
}

ptfe_primaryGeneratorAction::~ptfe_primaryGeneratorAction(){}

// Generate primary particles
void ptfe_primaryGeneratorAction::GeneratePrimaries(G4Event* event)
{

  // Get the detector (for metadata purposes)
  const ptfe_detectorConstruction* myDetector
      = static_cast<const ptfe_detectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  // Get a pointer to the surface (for MC purposes)

  G4String solidName;
  if(myDetector->ActivateRoughness()) solidName = G4String("RoughSurface");
  else solidName = G4String("RoughSurface");
  auto surface_solid = G4SolidStore::GetInstance()->GetSolid(solidName);
  G4ThreeVector surface_origin = G4PhysicalVolumeStore::GetInstance()->GetVolume("RoughSurface")->GetTranslation();

  //Generate one instance of specified particle
  G4int A = 210, Z = 84; // 210Po
  G4ParticleDefinition* ion
       = G4IonTable::GetIonTable()->GetIon(Z,A);
  fParticleGun->SetParticleDefinition(ion);
  fParticleGun->SetParticleCharge(0.);
  fParticleGun->SetParticleEnergy(0*eV);    


  // PARTICLE ORIGIN POINT
  // We don't generate decays in the liquid xenon. Start from the assumption we generated outside. Keep throwind random x,y,z until we end up inside the surface
  
  // CREATE RANGES FOR GENERATION
  // The farthest-travelling particle (alpha in liquid xenon) goes around for 46um
  // We want to generate on the surface (and deep beneath it) but surface should
  // be "infinite", so none of the alphas needs to reach the edges of the wall-box
  // For that reason, maximum space is box width - 2*46um (for each side).
  G4double x1  = -50*um;
  G4double x2  = 50*um;
  G4double y1  = -50*um;
  G4double y2  = 50*um;
  // Z goes from penetration depth to triangle height
  G4double z1  = -50*um;
  G4double z2  = 50*um;

  G4bool isDepthWrong = true;
  G4double x0, y0, z0;
  // x0 = G4UniformRand()*abs(x2-x1) + x1;
  // y0 = G4UniformRand()*abs(y2-y1) + y1;
  // z0 = G4UniformRand()*abs(z2-z1) + z1;
  G4int counter = 0;
  while(isDepthWrong)
  {
    // Generate random x,y point on the surface
    x0 = G4UniformRand()*abs(x2-x1) + x1;
    y0 = G4UniformRand()*abs(y2-y1) + y1;
    z0 = G4UniformRand()*abs(z2-z1) + z1;
    G4ThreeVector loc(x0,y0,z0);
    loc = loc - surface_origin;
    G4bool isInside = (surface_solid->Inside(loc)==1) || (surface_solid->Inside(loc)==2);
    G4bool isWithinContaminationDepth = (surface_solid->DistanceToOut(loc)<myDetector->ContaminationDepth());

    isDepthWrong = !(isInside && isWithinContaminationDepth);
    isDepthWrong = 0;
    counter++;

  }

  // Set particle position
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  fParticleGun->GeneratePrimaryVertex(event);
};
