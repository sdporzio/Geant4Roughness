#include "ptfe_detectorConstruction.hh"
#include "ptfe_materials.hh"
#include "ptfe_surfaceGenerator.hh"

#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"

#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4Material.hh"
#include "g4analysis.hh"

// Construct/Deconstructor
ptfe_detectorConstruction::ptfe_detectorConstruction() : G4VUserDetectorConstruction(),
  fMessenger(nullptr),
  fActivateRoughness(true),
  fWallWidth(100.*um),
  fWallDepth(100.*um),
  fFeaturesHeight(0.1*nm),
  fBaseWidth(0.),
  fFeaturesSpacing(0.),
  fContaminationDepth(100.*um),
  fParametrisedContamination(false)
{
  fMessenger = new G4GenericMessenger(this,"/ptfe/surface/","");
  fMessenger->DeclareProperty("activateRoughness",fActivateRoughness,"");
  fMessenger->DeclarePropertyWithUnit("wallWidth","um",fWallWidth,"");
  fMessenger->DeclarePropertyWithUnit("wallDepth","um",fWallDepth,"");
  fMessenger->DeclarePropertyWithUnit("baseWidth","um",fBaseWidth,"");
  fMessenger->DeclarePropertyWithUnit("featuresHeight","um",fFeaturesHeight,"");
  fMessenger->DeclarePropertyWithUnit("featuresSpacing","um",fFeaturesSpacing,"");
  fMessenger->DeclarePropertyWithUnit("contaminationDepth","um",fContaminationDepth,"");
  fMessenger->DeclareProperty("parametricContamination",fParametrisedContamination,"");

}
ptfe_detectorConstruction::~ptfe_detectorConstruction()
{
  delete fMessenger;
}

// Construct function
G4VPhysicalVolume* ptfe_detectorConstruction::Construct()
{
  // HARD-CODED SETTINGS
  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = false;

  // MATERIALS
  ptfe_materials* materials = new ptfe_materials();
  G4Material* in_material = materials->MakeTeflon();
  G4Material* out_material = materials->MakeLiquidXe();

  // WORLD
  // World is five times bigger than surface
  G4double world_sizeXY = fWallWidth*5;
  G4double world_sizeZ = fWallDepth*5;
  printf("World dimensions: %.1f x %.1f x %.1f um\n", world_sizeXY/um, world_sizeXY/um,world_sizeZ/um);
  // Build the world
  G4Box* solidWorld =    
    new G4Box("World",                                           //name
       world_sizeXY*0.5, world_sizeXY*0.5, world_sizeZ*0.5);     //size   
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,             //solid
                        out_material,           //material
                        "World");               //name                         
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //logical volume
                      "World",               //name
                      0,                     //mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
  // Give it a sensible colour
  G4VisAttributes* worldColour= new G4VisAttributes(G4Colour(0.2,0.2,0.9,0.1));
  logicWorld->SetVisAttributes(worldColour);


  // WALL
  ptfe_surfaceGenerator* sg = new ptfe_surfaceGenerator();
  G4VSolid* solidWall = sg->GenerateSurfaceSolid();
  G4LogicalVolume* logicWall =                         
    new G4LogicalVolume(solidWall,        //solid
                        in_material,         //material
                        "RoughSurface");          //name                              
  G4VPhysicalVolume* physWall = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0,0,-fWallDepth*0.5),
                      logicWall,             //logical volume
                      "RoughSurface",                //name
                      logicWorld,            //mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  // Give it a sensible colour
  G4VisAttributes* wallColour= new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
  logicWall->SetVisAttributes(wallColour);

  printf("Detector built.\n");

  return physWorld;
}
