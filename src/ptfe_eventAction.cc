#include "ptfe_eventAction.hh"
#include "ptfe_runAction.hh"
#include "Randomize.hh"
#include "g4analysis.hh"
#include "G4Event.hh"

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


}     

void ptfe_eventAction::EndOfEventAction(const G4Event* event)
{
  // DIAGNOSTIC MESSAGE
  printf("-> ENERGY DEPOSIT\n");
  // printf("World number deposits: %i\n",(int) fEnDep_vector["World"].size());
  // printf("World total energy deposit: %.2f\n",fEnDep["World"]);
  // printf("Cushion number deposits: %i\n",(int) fEnDep_vector["Cushion"].size());
  // printf("Cushion total energy deposit: %.2f\n",fEnDep["Cushion"]);
  // printf("Wall number deposits: %i\n",(int) fEnDep_vector["Wall"].size());
  // printf("Wall total energy deposit: %.2f\n",fEnDep["Wall"]);
  // printf("RoughSurface number deposits: %i\n",(int) fEnDep_vector["RoughSurface"].size());
  printf("RoughSurface total energy deposit: %.2f\n",fEnDep["RoughSurface"]);

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

}