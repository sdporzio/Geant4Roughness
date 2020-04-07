#include "ptfe_eventAction.hh"
#include "ptfe_runAction.hh"
#include "ptfe_anaTrack.hh"
#include "Randomize.hh"
#include "g4analysis.hh"
#include "G4Event.hh"
#include "G4VTrajectory.hh"

ptfe_eventAction::ptfe_eventAction(ptfe_runAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction)
{}
ptfe_eventAction::~ptfe_eventAction(){}

void ptfe_eventAction::BeginOfEventAction(const G4Event* event)
{
  // Get the random seeds for this event
  fEventSeedIndex = CLHEP::HepRandom::getTheSeed();
  fEventSeed1 = CLHEP::HepRandom::getTheSeeds()[0];
  fEventSeed2 = CLHEP::HepRandom::getTheSeeds()[1];

  // Reset all the variables
  fEnDep["World"] = 0;
  fEnDep["Cushion"] = 0;
  fEnDep["Wall"] = 0;
  fEnDep["RoughSurface"] = 0;

  fEnDep_vector["World"].clear();
  fEnDep_vector["Cushion"].clear();
  fEnDep_vector["Wall"].clear();
  fEnDep_vector["RoughSurface"].clear();

  fAnaTrack.clear();
}     

void ptfe_eventAction::EndOfEventAction(const G4Event* event)
{
  // printf("SUMMARY OF EVENT %i\n", (int) event->GetEventID());
  // G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
  // G4int n_trajectories = 0;
  // if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
  // printf("Number tracks: %i\n", n_trajectories);
  // for(int i=0; i<n_trajectories; i++)
  // {
  //   G4VTrajectory* currentTrack = (*trajectoryContainer)[i];
  //   G4int tid = currentTrack->GetTrackID();
  //   G4int gid = currentTrack->GetParentID();
  //   G4String pname = currentTrack->GetParticleName();
  //   G4int pdg = currentTrack->GetPDGEncoding();

  //   printf("%s (%i) [TrackID: %i; ParentID: %i]\n", pname.c_str(),pdg,tid,gid);
  // }

  printf("RoughSurface total energy deposit: %.2f MeV\n",fEnDep["RoughSurface"]+fEnDep["Wall"]);
  printf("External total energy deposit: %.2f MeV\n",fEnDep["Cushion"]+fEnDep["World"]);

  // Save metadata to the ROOT file
  auto analysisManager = G4AnalysisManager::Instance(); G4int i = 0;
  analysisManager->FillNtupleIColumn(1,i,event->GetEventID()); i+=1;
  analysisManager->FillNtupleIColumn(1,i,fEventSeedIndex); i+=1;
  analysisManager->FillNtupleIColumn(1,i,fEventSeed1); i+=1;
  analysisManager->FillNtupleIColumn(1,i,fEventSeed2); i+=1;

  analysisManager->FillNtupleIColumn(1,i,fEnDep_vector["World"].size()); i+=1;
  analysisManager->FillNtupleIColumn(1,i,fEnDep_vector["Cushion"].size()); i+=1;
  analysisManager->FillNtupleIColumn(1,i,fEnDep_vector["Wall"].size()); i+=1;
  analysisManager->FillNtupleIColumn(1,i,fEnDep_vector["RoughSurface"].size()); i+=1;
  analysisManager->FillNtupleIColumn(1,i,fEnDep_vector["Wall"].size()+fEnDep_vector["RoughSurface"].size()); i+=1;
  analysisManager->FillNtupleIColumn(1,i,fEnDep_vector["World"].size()+fEnDep_vector["Cushion"].size()); i+=1;

  analysisManager->FillNtupleDColumn(1,i,fEnDep["World"]); i+=1;
  analysisManager->FillNtupleDColumn(1,i,fEnDep["Cushion"]); i+=1;
  analysisManager->FillNtupleDColumn(1,i,fEnDep["Wall"]); i+=1;
  analysisManager->FillNtupleDColumn(1,i,fEnDep["RoughSurface"]); i+=1;
  analysisManager->FillNtupleDColumn(1,i,fEnDep["Wall"]+fEnDep["RoughSurface"]); i+=1;
  analysisManager->FillNtupleDColumn(1,i,fEnDep["World"]+fEnDep["Cushion"]); i+=1;

  analysisManager->AddNtupleRow(1);


  // Fill the track level information
  G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
  for(int k=0; k<n_trajectories; k++)
  {
    G4int j = 0;
    G4VTrajectory* currentTrack = (*trajectoryContainer)[k];
    G4int tid = currentTrack->GetTrackID();
    G4int gid = currentTrack->GetParentID();
    G4String pname = currentTrack->GetParticleName();
    G4int pdg = currentTrack->GetPDGEncoding();

    analysisManager->FillNtupleIColumn(2,j,event->GetEventID()); j+=1;
    analysisManager->FillNtupleSColumn(2,j,pname); j+=1;
    analysisManager->FillNtupleIColumn(2,j,pdg); j+=1;
    analysisManager->FillNtupleIColumn(2,j,tid); j+=1;
    analysisManager->FillNtupleIColumn(2,j,gid); j+=1;
    analysisManager->AddNtupleRow(2);
  }







  printf("\n");
}