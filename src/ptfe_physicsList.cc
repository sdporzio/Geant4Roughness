#include "ptfe_physicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmLowEPPhysics.hh"
#include "G4EmStandardPhysicsSS.hh"
#include "G4EmStandardPhysicsWVI.hh"
#include "G4EmStandardPhysicsGS.hh"

#include "G4IonPhysics.hh"


ptfe_physicsList::ptfe_physicsList() 
: G4VModularPhysicsList(){
  SetVerboseLevel(1);

  // Default physics
  RegisterPhysics(new G4DecayPhysics());

  // EM physics
  RegisterPhysics(new G4EmStandardPhysics());
  RegisterPhysics(new G4EmLivermorePhysics());

  // Radioactive decay
  RegisterPhysics(new G4RadioactiveDecayPhysics());

  // EM physics
  RegisterPhysics(new G4EmLowEPPhysics());
  RegisterPhysics(new G4EmStandardPhysicsSS());
  RegisterPhysics(new G4EmStandardPhysicsWVI());
  RegisterPhysics(new G4EmStandardPhysicsGS());
  RegisterPhysics(new G4IonPhysics());


}

ptfe_physicsList::~ptfe_physicsList(){}

void ptfe_physicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}

void ptfe_physicsList::AddIonGasModels()
{
  // G4EmConfigurator* em_config = 
  //   G4LossTableManager::Instance()->EmConfigurator();
  // auto particleIterator=GetParticleIterator();
  // particleIterator->reset();
  // while ((*particleIterator)())
  // {
  //   G4ParticleDefinition* particle = particleIterator->value();
  //   G4String partname = particle->GetParticleName();
  //   if(partname == "alpha" || partname == "He3" || partname == "GenericIon") {
  //     G4BraggIonGasModel* mod1 = new G4BraggIonGasModel();
  //     G4BetheBlochIonGasModel* mod2 = new G4BetheBlochIonGasModel();
  //     G4double eth = 2.*MeV*particle->GetPDGMass()/proton_mass_c2;
  //     em_config->SetExtraEmModel(partname,"ionIoni",mod1,"",0.0,eth,
  //                                new G4IonFluctuations());
  //     em_config->SetExtraEmModel(partname,"ionIoni",mod2,"",eth,100*TeV,
  //                                new G4UniversalFluctuation());

  //   }
  // }
}