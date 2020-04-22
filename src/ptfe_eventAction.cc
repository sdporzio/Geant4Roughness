
#include "ptfe_eventAction.hh"
#include "ptfe_runAction.hh"
#include "ptfe_anaTrack.hh"
#include "Randomize.hh"
#include "g4analysis.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
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
  G4bool isEventKilled = G4RunManager::GetRunManager()->GetCurrentEvent()->IsAborted();
  if(isEventKilled) return;

  // const G4Run* thisRun = G4RunManager::GetRunManager()->GetCurrentRun();
  G4int totNumber = G4RunManager::GetRunManager()->GetNumberOfEventsToBeProcessed();
  G4int eNumber = event->GetEventID();
  G4float perc = (eNumber/(float) totNumber)*100.;
  if((int) (perc*10000)%200000==0)
  {
    printf("Processing event %i/%i [%.0f%%]\n",eNumber,totNumber,perc);
  }



  // printf("SUMMARY OF EVENT %i\n", (int) event->GetEventID());
  G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
  // printf("Number tracks: %i\n", n_trajectories);
  for(int i=0; i<n_trajectories; i++)
  {
    G4VTrajectory* currentTrack = (*trajectoryContainer)[i];
    G4int tid = currentTrack->GetTrackID();
    G4int gid = currentTrack->GetParentID();
    G4String pname = currentTrack->GetParticleName();
    G4int pdg = currentTrack->GetPDGEncoding();

    // printf("%s (%i) [TrackID: %i; ParentID: %i]\n", pname.c_str(),pdg,tid,gid);
  }

  //primary positions: Alvine to Davide
  G4PrimaryVertex* pvertex=event->GetPrimaryVertex();
  G4ThreeVector vtx=pvertex->GetPosition();
  // cout<<"--------> primary positions to verify events location   "<<vtx[0]/nm <<"    "<<vtx[1]/nm <<"    "<<vtx[2]/nm <<endl;

  // Save metadata to the ROOT file
  auto analysisManager = G4AnalysisManager::Instance(); 
  G4int i = 0;
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
    analysisManager->FillNtupleDColumn(2,j,thisTrack.distTravelled); j+=1;
    analysisManager->FillNtupleDColumn(2,j,thisTrack.enDeposited_tot); j+=1;
    analysisManager->FillNtupleDColumn(2,j,thisTrack.enDeposited_wall); j+=1;
    analysisManager->FillNtupleDColumn(2,j,thisTrack.enDeposited_surface); j+=1;
    analysisManager->FillNtupleDColumn(2,j,thisTrack.enDeposited_cushion); j+=1;
    analysisManager->FillNtupleDColumn(2,j,thisTrack.enDeposited_world); j+=1;
    analysisManager->FillNtupleDColumn(2,j,thisTrack.enDeposited_wall+thisTrack.enDeposited_surface); j+=1;
    analysisManager->FillNtupleDColumn(2,j,thisTrack.enDeposited_cushion+thisTrack.enDeposited_world); j+=1;

    // Save this row
    analysisManager->AddNtupleRow(2);
  }

  // Print some generic information out
  //printf("RoughSurface total energy deposit: %.2f keV\n",fEnDep["RoughSurface"]+fEnDep["Wall"]); //Alvine to Davide, units here are wrong, it should be keV not MeV
  // printf("External total energy deposit: %.2f keV\n",fEnDep["Cushion"]+fEnDep["World"]); //Alvine to Davide, units here are wrong, it should be keV not MeV
  //myfile1<<(fEnDep["RoughSurface"]+fEnDep["Wall"]);
  // printf("\n");
}
