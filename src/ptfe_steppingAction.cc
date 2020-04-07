#include "ptfe_steppingAction.hh"
#include "ptfe_eventAction.hh"
#include "ptfe_detectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4StepPoint.hh"

ptfe_steppingAction::ptfe_steppingAction(ptfe_eventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction)
{}

ptfe_steppingAction::~ptfe_steppingAction()
{}

void ptfe_steppingAction::UserSteppingAction(const G4Step* step)
{

  G4StepPoint* point = step->GetPreStepPoint();
  G4Track* track = step->GetTrack();

  G4int track_id = track->GetTrackID();
  G4String pname = track->GetParticleDefinition()->GetParticleName();

  // fEventAction->fAnaTrack[track_id] = new ptfe_anaTrptfe_anaTrack();
  // fEventAction->fAnaTrack[track_id].trackId = track_id;
  // fEventAction->fAnaTrack[track_id].name = pname;

  // Volume name
  G4LogicalVolume* volume = point->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
  G4String volumeName = volume->GetName();
  // Energy deposit
  G4double edep = step->GetTotalEnergyDeposit();
  fEventAction->AddEnDep(volumeName,edep);
}
