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
  analysisManager->CreateNtuple("metadata", "Metadata"); // ntuple-ID 0
  analysisManager->CreateNtupleIColumn("surface_activateRoughness"); //c-ID 0
  analysisManager->CreateNtupleIColumn("surface_numberFeaturesSide"); //c-ID 1
  analysisManager->CreateNtupleDColumn("surface_baseWidth"); //c-ID 2
  analysisManager->CreateNtupleDColumn("surface_featuresHeight"); //c-ID 3
  analysisManager->CreateNtupleDColumn("surface_featuresSpacing"); //c-ID 4
  analysisManager->FinishNtuple();

  // Create the data branch
  analysisManager->CreateNtuple("data", "Data"); // ntuple-ID 1
  analysisManager->CreateNtupleIColumn("Dc1Hits");  //c-ID 0
  analysisManager->CreateNtupleIColumn("Dc2Hits");  //c-ID 1
  analysisManager->CreateNtupleDColumn("ECEnergy"); //c-ID 2
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
  analysisManager->FillNtupleIColumn(0,i,myDetector->ActivateRoughness()); i+=1;
  analysisManager->FillNtupleIColumn(0,i,myDetector->NumberFeaturesSide()); i+=1;
  analysisManager->FillNtupleDColumn(0,i,myDetector->BaseWidth()); i+=1;
  analysisManager->FillNtupleDColumn(0,i,myDetector->FeaturesHeight()); i+=1;
  analysisManager->FillNtupleDColumn(0,i,myDetector->FeaturesSpacing()); i+=1;
  analysisManager->AddNtupleRow(0);
  analysisManager->Write();
  analysisManager->CloseFile();
}