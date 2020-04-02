// G4 INCLUDES
#include "G4RunManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"
#include "Randomize.hh"
// PROJECTS INCLUDE
#include "ptfe_detectorConstruction.hh"
#include "ptfe_actionInitialization.hh"
#include "ptfe_surfaceParameterisation.hh"


// MAIN FUNCTION
int main(int argc,char** argv)
{
  // INITIALIZE THE UI CLASS
  G4UIExecutive* ui = 0;
  ui = new G4UIExecutive(argc, argv);

  // DEFAULT RUN MANAGER INITIALIZATION
  G4RunManager* runManager = new G4RunManager;

  // MANDATORY USER INITIALIZATION CLASSES
  // Detector class
  runManager->SetUserInitialization(new ptfe_detectorConstruction());
  // Physics list class
  G4VModularPhysicsList* physicsList = new QBBC;
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);
  // Action class
  runManager->SetUserInitialization(new ptfe_actionInitialization());


  // OPTIONAL CLASSES
  // Visualization class
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // UI class
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  UImanager->ApplyCommand("/control/execute initialize.mac");
  ui->SessionStart();

  // BATCH RUN
  // runManager->Initialize();
  // runManager->BeamOn(1);

  printf("All good so far.\n");

  delete ui;
  delete visManager;
  delete runManager;
}
