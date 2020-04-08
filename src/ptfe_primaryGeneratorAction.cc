#include "ptfe_primaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
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
  //Generate one instance of specified particle
  G4int A = 210, Z = 84; // 210Po
  G4ParticleDefinition* ion
       = G4IonTable::GetIonTable()->GetIon(Z,A);
  fParticleGun->SetParticleDefinition(ion);
  fParticleGun->SetParticleCharge(0.);
  fParticleGun->SetParticleEnergy(0*eV);    


  G4double x0  = 0*um, y0  = 0*um, z0  = 5*um;
  G4double dx0 = 1*um, dy0 = 1*um, dz0 = 0*um; 
  x0 += dx0*(G4UniformRand()-dx0/2.);
  y0 += dy0*(G4UniformRand()-dy0/2.);
  z0 += dz0*(G4UniformRand()-dz0/2.);
  // fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  fParticleGun->SetParticlePosition(G4ThreeVector(0,0,0));

  fParticleGun->GeneratePrimaryVertex(event);
};