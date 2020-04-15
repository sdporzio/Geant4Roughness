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


ptfe_primaryGeneratorAction::ptfe_primaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleSource(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);
  // fParticleSource = new G4GeneralParticleSource();
  // G4String particleName;
  // G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  // G4ParticleDefinition* particle
    // = particleTable->FindParticle(particleName="gamma");
  // fParticleSource->GetCurrentSource()->SetParticleDefinition(particle);
}

ptfe_primaryGeneratorAction::~ptfe_primaryGeneratorAction(){}

// Generate primary particles
void ptfe_primaryGeneratorAction::GeneratePrimaries(G4Event* event)
{

  // Get the detector (for metadata purposes)
  const ptfe_detectorConstruction* myDetector
      = static_cast<const ptfe_detectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  //Generate one instance of specified particle
  G4int A = 210, Z = 84; // 210Po
  G4ParticleDefinition* ion
       = G4IonTable::GetIonTable()->GetIon(Z,A);
  fParticleGun->SetParticleDefinition(ion);
  fParticleGun->SetParticleCharge(0.);
  fParticleGun->SetParticleEnergy(0*eV);    

  // PARTICLE ORIGIN POINT
  // The farthest-travelling particle (alpha in liquid xenon) goes around for 46um
  // We want to generate on the surface (and deep beneath it) but surface should be
  // "infinite", so none of the alphas needs to reach the edges of the wall-box
  // For that reason, maximum space is box width - 2*46um (for each side).
  G4double x1  = -(myDetector->WallWidth()/2.) + 46*CLHEP::um;
  G4double x2  = (myDetector->WallWidth()/2.) - 46*CLHEP::um;

  G4double y1  = -(myDetector->WallWidth()/2.) + 46*CLHEP::um;
  G4double y2  = (myDetector->WallWidth()/2.) - 46*CLHEP::um;

  G4double z1  = (myDetector->WallDepth()/2.0) - (myDetector->ContaminationDepth()); 
  G4double z2  = (myDetector->WallDepth()/2.0) + (myDetector->FeaturesHeight());


  G4double x0 = G4UniformRand()*abs(x2-x1) + x1;
  G4double y0 = G4UniformRand()*abs(y2-y1) + y1; 
  G4double z0 = G4UniformRand()*abs(z2-z1) + z1; 

  // G4double x0 =-(myDetector->WallWidth()/2.) + (myDetector->WallWidth())*G4UniformRand();
  //G4double y0 =-(myDetector->WallWidth()/2.) + (myDetector->WallWidth())*G4UniformRand();
  
  //cout<<x0/nm <<"   "<<y0/nm <<"   "<<z0/nm <<endl;
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));



  fParticleGun->GeneratePrimaryVertex(event);
};
