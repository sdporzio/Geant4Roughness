// G4 INCLUDES
#include "G4RunManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"
#include "Randomize.hh"
// PROJECTS INCLUDE
#include "ptfe_detectorConstruction.hh"
#include "ptfe_physicsList.hh"
#include "ptfe_actionInitialization.hh"


// MAIN FUNCTION
int main(int argc,char** argv)
{
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
  CLHEP::HepRandom::setTheSeed(121);
  G4bool visual = false;
  G4int nEvents = 1000000;

  // INITIALIZE THE UI CLASS
  G4UIExecutive* ui = 0;
  ui = new G4UIExecutive(argc, argv);

  // DEFAULT RUN MANAGER INITIALIZATION
  G4RunManager* runManager = new G4RunManager;

  // MANDATORY USER INITIALIZATION CLASSES
  // Detector class
  ptfe_detectorConstruction* myDetector = new ptfe_detectorConstruction();
  runManager->SetUserInitialization(myDetector);
  // Physics list class
  // G4VModularPhysicsList* myPhysicsList = new QBBC;
  G4VModularPhysicsList* myPhysicsList = new ptfe_physicsList();
  myPhysicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(myPhysicsList);
  // Action class
  ptfe_actionInitialization* myAction = new ptfe_actionInitialization();
  runManager->SetUserInitialization(myAction);

  // OPTIONAL CLASSES
  // Visualization class
  G4VisManager* visManager = new G4VisExecutive;
  if(visual) visManager->Initialize();

  // UI class
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  UImanager->ApplyCommand("/control/execute initialize.mac");
  if(visual) ui->SessionStart(); // Uncomment this to enable UI

  // BATCH RUN
  // Run simulation
  if(!visual)
  {
    runManager->Initialize();
    runManager->BeamOn(nEvents); 
  }

  delete ui;
  delete runManager;
  delete visManager;


}
