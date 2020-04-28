#include "ptfe_primaryGeneratorAction.hh"
#include "ptfe_detectorConstruction.hh"
#include "ptfe_surfaceGenerator.hh"

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
  fParticleGun1  = new G4ParticleGun(n_particle);
  fParticleGun2  = new G4ParticleGun(n_particle);
}

ptfe_primaryGeneratorAction::~ptfe_primaryGeneratorAction(){}

// Generate primary particles
void ptfe_primaryGeneratorAction::GeneratePrimaries(G4Event* event)
{

  // Get the detector (for metadata purposes)
  const ptfe_detectorConstruction* myDetector
      = static_cast<const ptfe_detectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  ptfe_surfaceGenerator* sg = myDetector->GetSG();
  G4double x1 = -1*um;
  G4double x2 = 1*um;
  G4double y1 = sg->GetMinY() - 0.2*um;
  G4double y2 = sg->GetMaxY();
  G4double z1 = sg->GetMinX() + 20*um;
  G4double z2 = sg->GetMaxX() - 20*um;

  // Get a pointer to the surface (for MC purposes)
  auto surface_solid = G4SolidStore::GetInstance()->GetSolid("RoughSurface");
  G4ThreeVector surface_origin = G4PhysicalVolumeStore::GetInstance()->GetVolume("RoughSurface")->GetTranslation();

  // //Generate one instance of specified particle
  // G4int A = 210, Z = 84; // 210Po
  // G4ParticleDefinition* ion
  //      = G4IonTable::GetIonTable()->GetIon(Z,A);
  // fParticleGun->SetParticleDefinition(ion);
  // fParticleGun->SetParticleCharge(0.);
  // fParticleGun->SetParticleEnergy(0*eV);

  // Alternatively, generate directly the decay products
  G4String particleName;
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* alpha
    = particleTable->FindParticle(particleName="alpha");
  fParticleGun1->SetParticleDefinition(alpha);
  fParticleGun1->SetParticleEnergy(5.304307*MeV);

  G4int A = 206, Z = 82; // 206Pb
  G4ParticleDefinition* lead
       = G4IonTable::GetIonTable()->GetIon(Z,A);
  fParticleGun2->SetParticleDefinition(lead);
  fParticleGun2->SetParticleEnergy(0.103143*MeV);


  // PARTICLE ORIGIN POINT
  // We don't generate decays in the liquid xenon. Start from the assumption we generated outside. Keep throwind random x,y,z until we end up inside the surface
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
    G4bool isInside = (surface_solid->Inside(loc)==2);
    G4bool isWithinContaminationDepth = (surface_solid->DistanceToOut(loc)<myDetector->GetContaminationDepth());

    // isDepthWrong = false;
    isDepthWrong = !(isInside && isWithinContaminationDepth);
    counter++;

  }

  // Set particle position
  // printf("%.1f, %.1f, %.1f\n\n\n", x0/um, y0/um, z0/um);
  // fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  // fParticleGun->GeneratePrimaryVertex(event);

  fParticleGun1->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  fParticleGun1->SetParticleMomentumDirection(G4ThreeVector(0,1,0));
  fParticleGun1->GeneratePrimaryVertex(event);

  fParticleGun2->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  fParticleGun2->SetParticleMomentumDirection(G4ThreeVector(0,-1,0));
  fParticleGun2->GeneratePrimaryVertex(event);
};
