#include "ptfe_primaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Proton.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

ptfe_primaryGeneratorAction::ptfe_primaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleSource(0)
{
  fParticleSource = new G4GeneralParticleSource();
  G4String particleName;
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="gamma");
  fParticleSource->GetCurrentSource()->SetParticleDefinition(particle);
}
ptfe_primaryGeneratorAction::~ptfe_primaryGeneratorAction(){}

// Generate primary particles
void ptfe_primaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  //Generate one instance of specified particle
  fParticleSource->GeneratePrimaryVertex(event);
};