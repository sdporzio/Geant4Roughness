#include "ptfe_steppingAction.hh"
#include "ptfe_eventAction.hh"
#include "ptfe_detectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ptfe_steppingAction::ptfe_steppingAction(ptfe_eventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction)
{}

ptfe_steppingAction::~ptfe_steppingAction()
{}

void ptfe_steppingAction::UserSteppingAction(const G4Step* step)
{

  // get volume of the current step
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
      
  printf("Track ID: %i\n",step->GetTrack()->GetTrackID());
  printf("Volume name: %s\n\n",volume->GetName().c_str());

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

