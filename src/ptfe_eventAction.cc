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
  printf("SUMMARY OF EVENT %i\n", (int) event->GetEventID());

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

  // TRACK INFORMATION TIME
  G4int n_trajectories = fAnaTrack.size();
  printf("Number tracks: %i\n", n_trajectories);
  for ( const auto &thisId : fAnaTrack )
  {
    G4int j = 0;
    auto thisTrack = fAnaTrack[thisId.first];

    analysisManager->FillNtupleIColumn(2,j,event->GetEventID()); j+=1;
    analysisManager->FillNtupleSColumn(2,j,thisTrack.name); j+=1;
    analysisManager->FillNtupleIColumn(2,j,thisTrack.pdg); j+=1;
    analysisManager->FillNtupleIColumn(2,j,thisTrack.trackId); j+=1;
    analysisManager->FillNtupleIColumn(2,j,thisTrack.parentId); j+=1;
    // Coordinates start
    analysisManager->FillNtupleDColumn(2,j,thisTrack.xStart); j+=1;
    analysisManager->FillNtupleDColumn(2,j,thisTrack.yStart); j+=1;
    analysisManager->FillNtupleDColumn(2,j,thisTrack.zStart); j+=1;
    // Momentum start
    analysisManager->FillNtupleDColumn(2,j,thisTrack.pxStart); j+=1;
    analysisManager->FillNtupleDColumn(2,j,thisTrack.pyStart); j+=1;
    analysisManager->FillNtupleDColumn(2,j,thisTrack.pzStart); j+=1;
    // Time-energy start
    analysisManager->FillNtupleDColumn(2,j,thisTrack.eStart); j+=1;
    analysisManager->FillNtupleDColumn(2,j,thisTrack.kStart); j+=1;
    analysisManager->FillNtupleDColumn(2,j,thisTrack.tStart); j+=1;
    // Coordinates end
    analysisManager->FillNtupleDColumn(2,j,thisTrack.xEnd); j+=1;
    analysisManager->FillNtupleDColumn(2,j,thisTrack.yEnd); j+=1;
    analysisManager->FillNtupleDColumn(2,j,thisTrack.zEnd); j+=1;
    // Momentum end
    analysisManager->FillNtupleDColumn(2,j,thisTrack.pxEnd); j+=1;
    analysisManager->FillNtupleDColumn(2,j,thisTrack.pyEnd); j+=1;
    analysisManager->FillNtupleDColumn(2,j,thisTrack.pzEnd); j+=1;
    // Time-energy end
    analysisManager->FillNtupleDColumn(2,j,thisTrack.eEnd); j+=1;
    analysisManager->FillNtupleDColumn(2,j,thisTrack.kEnd); j+=1;
    analysisManager->FillNtupleDColumn(2,j,thisTrack.tEnd); j+=1;
    // Volume information
    analysisManager->FillNtupleSColumn(2,j,thisTrack.volumeStart); j+=1;
    analysisManager->FillNtupleSColumn(2,j,thisTrack.volumeEnd); j+=1;
    // Energy deposited
    analysisManager->FillNtupleDColumn(2,j,thisTrack.enDeposited); j+=1;    

    // Save this row
    analysisManager->AddNtupleRow(2);

    // Print some information out
    printf("%s (%i) [TrackID: %i; ParentID: %i]\n", thisTrack.name.c_str(),thisTrack.pdg,thisTrack.trackId,thisTrack.parentId);
  }

  // Print some generic information out
  printf("RoughSurface total energy deposit: %.2f MeV\n",fEnDep["RoughSurface"]+fEnDep["Wall"]);
  printf("External total energy deposit: %.2f MeV\n",fEnDep["Cushion"]+fEnDep["World"]);

  printf("\n");
}