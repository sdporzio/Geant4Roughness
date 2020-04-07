#include "ptfe_physicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"


ptfe_physicsList::ptfe_physicsList() 
: G4VModularPhysicsList(){
  SetVerboseLevel(1);

  // Default physics
  RegisterPhysics(new G4DecayPhysics());

  // EM physics
  RegisterPhysics(new G4EmStandardPhysics());

  // Radioactive decay
  RegisterPhysics(new G4RadioactiveDecayPhysics());
}

ptfe_physicsList::~ptfe_physicsList(){}

void ptfe_physicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}  