#include "ptfe_actionInitialization.hh"
#include "ptfe_primaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4Proton.hh"
#include "G4SystemOfUnits.hh"

// Construct/Deconstructor
ptfe_actionInitialization::ptfe_actionInitialization() : G4VUserActionInitialization(){};
ptfe_actionInitialization::~ptfe_actionInitialization(){};

void ptfe_actionInitialization::Build() const
{
  SetUserAction(new ptfe_primaryGeneratorAction);
}
