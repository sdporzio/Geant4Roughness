#include "ptfe_runAction.hh"
#include "ptfe_detectorConstruction.hh"
#include "G4GenericMessenger.hh"
#include "G4RunManager.hh"
#include "g4analysis.hh"


ptfe_runAction::ptfe_runAction() : G4UserRunAction()
{

  // Create the root manager
  auto analysisManager = G4Analysis::ManagerInstance("root");
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Default settings
  analysisManager->SetNtupleMerging(true);
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFileName("ptfe");

  // Create the metadata branch
  analysisManager->CreateNtuple("metadata", "Metadata");              //ntuple-ID 0
  analysisManager->CreateNtupleIColumn("seed_run");
  analysisManager->CreateNtupleIColumn("surface_activateRoughness");
  analysisManager->CreateNtupleDColumn("surface_wallWidth");
  analysisManager->CreateNtupleDColumn("surface_wallDepth");
  analysisManager->CreateNtupleIColumn("surface_numberFeaturesSide");
  analysisManager->CreateNtupleDColumn("surface_baseWidth");        
  analysisManager->CreateNtupleDColumn("surface_featuresHeight");   
  analysisManager->CreateNtupleDColumn("surface_featuresSpacing");  
  analysisManager->FinishNtuple();

  // Create the data branch
  analysisManager->CreateNtuple("event_data", "EventData");             //ntuple-ID 1
  analysisManager->CreateNtupleIColumn("event_number");
  analysisManager->CreateNtupleIColumn("seed_event_index");
  analysisManager->CreateNtupleIColumn("seed_event1");
  analysisManager->CreateNtupleIColumn("seed_event2");

  analysisManager->CreateNtupleIColumn("nhits_world");  
  analysisManager->CreateNtupleIColumn("nhits_cushion");
  analysisManager->CreateNtupleIColumn("nhits_wall");   
  analysisManager->CreateNtupleIColumn("nhits_surface");
  analysisManager->CreateNtupleIColumn("nhits_in");     
  analysisManager->CreateNtupleIColumn("nhits_out"); 

  analysisManager->CreateNtupleDColumn("edep_world");   
  analysisManager->CreateNtupleDColumn("edep_cushion"); 
  analysisManager->CreateNtupleDColumn("edep_wall");    
  analysisManager->CreateNtupleDColumn("edep_surface"); 
  analysisManager->CreateNtupleDColumn("edep_in");       
  analysisManager->CreateNtupleDColumn("edep_out");      

  // Create the tracks branch
  analysisManager->CreateNtuple("tracks_data", "TracksData");             //ntuple-ID 2
  analysisManager->CreateNtupleIColumn("event_number");
  analysisManager->CreateNtupleSColumn("particle_name");
  analysisManager->CreateNtupleIColumn("particle_pdg");
  analysisManager->CreateNtupleIColumn("tracks_id");
  analysisManager->CreateNtupleIColumn("parent_id");
  // Coordinates start
  analysisManager->CreateNtupleDColumn("xStart");
  analysisManager->CreateNtupleDColumn("yStart");
  analysisManager->CreateNtupleDColumn("zStart");
  // Momentum start
  analysisManager->CreateNtupleDColumn("pxStart");
  analysisManager->CreateNtupleDColumn("pyStart");
  analysisManager->CreateNtupleDColumn("pzStart");
  // Time-energy start
  analysisManager->CreateNtupleDColumn("eStart");
  analysisManager->CreateNtupleDColumn("kStart");
  analysisManager->CreateNtupleDColumn("tStart");
  // Coordinates end
  analysisManager->CreateNtupleDColumn("xEnd");
  analysisManager->CreateNtupleDColumn("yEnd");
  analysisManager->CreateNtupleDColumn("zEnd");
  // Momentum end
  analysisManager->CreateNtupleDColumn("pxEnd");
  analysisManager->CreateNtupleDColumn("pyEnd");
  analysisManager->CreateNtupleDColumn("pzEnd");
  // Time-energy end
  analysisManager->CreateNtupleDColumn("eEnd");
  analysisManager->CreateNtupleDColumn("kEnd");
  analysisManager->CreateNtupleDColumn("tEnd");
  // Volume information
  analysisManager->CreateNtupleSColumn("volumeStart");
  analysisManager->CreateNtupleSColumn("volumeEnd");
  // Energy deposited
  analysisManager->CreateNtupleDColumn("distTravelled");
  analysisManager->CreateNtupleDColumn("enDeposited_tot");
  analysisManager->CreateNtupleDColumn("enDeposited_wall");
  analysisManager->CreateNtupleDColumn("enDeposited_surface");
  analysisManager->CreateNtupleDColumn("enDeposited_cushion");
  analysisManager->CreateNtupleDColumn("enDeposited_world");
  analysisManager->CreateNtupleDColumn("enDeposited_in");
  analysisManager->CreateNtupleDColumn("enDeposited_out");

  analysisManager->FinishNtuple();
}

ptfe_runAction::~ptfe_runAction()
{}

void ptfe_runAction::BeginOfRunAction(const G4Run*)
{
  // Create ROOT file
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile();
}     

void ptfe_runAction::EndOfRunAction(const G4Run* run)
{
  // Get the detector (for metadata purposes)
  const ptfe_detectorConstruction* myDetector
      = static_cast<const ptfe_detectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  // Save metadata to the ROOT file
  auto analysisManager = G4AnalysisManager::Instance(); G4int i = 0;
  analysisManager->FillNtupleIColumn(0,i,CLHEP::HepRandom::getTheSeed()); i+=1;
  analysisManager->FillNtupleIColumn(0,i,myDetector->ActivateRoughness()); i+=1;
  analysisManager->FillNtupleDColumn(0,i,myDetector->WallWidth()); i+=1;
  analysisManager->FillNtupleDColumn(0,i,myDetector->WallDepth()); i+=1;
  analysisManager->FillNtupleIColumn(0,i,myDetector->NumberFeaturesSide()); i+=1;
  analysisManager->FillNtupleDColumn(0,i,myDetector->BaseWidth()); i+=1;
  analysisManager->FillNtupleDColumn(0,i,myDetector->FeaturesHeight()); i+=1;
  analysisManager->FillNtupleDColumn(0,i,myDetector->FeaturesSpacing()); i+=1;
  analysisManager->AddNtupleRow(0);
  analysisManager->Write();
  analysisManager->CloseFile();
}