#include "ptfe_detectorConstruction.hh"
#include "ptfe_surfaceParameterisation.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4GenericMessenger.hh"

#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4Trd.hh"

// Construct/Deconstructor
ptfe_detectorConstruction::ptfe_detectorConstruction() : G4VUserDetectorConstruction(),
  fMessenger(nullptr),
  fActivateRoughness(true),
  fNumberFeaturesSide(10),
  fFeaturesHeight(10),
  fBaseWidth(30),
  fFeaturesSpacing(0.5)
{
  fMessenger = new G4GenericMessenger(this,"/ptfe/surface/","...");
  fMessenger->DeclareProperty("activateRoughness",fActivateRoughness,"...");
  fMessenger->DeclareProperty("numberFeaturesSide",fNumberFeaturesSide,"...");
  fMessenger->DeclarePropertyWithUnit("baseWidth","um",fBaseWidth,"...");
  fMessenger->DeclarePropertyWithUnit("featuresHeight","um",fFeaturesHeight,"...");
  fMessenger->DeclarePropertyWithUnit("featuresSpacing","um",fFeaturesSpacing,"...");

}
ptfe_detectorConstruction::~ptfe_detectorConstruction()
{
  delete fMessenger;
}

// Construct function
G4VPhysicalVolume* ptfe_detectorConstruction::Construct()
{
  printf("Params:%i,%.1f,%.1f,%.1f\n",fNumberFeaturesSide,fFeaturesSpacing,fFeaturesHeight,fBaseWidth);
  // HARD-CODED SETTINGS
  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;

  // NIST MATERIALS
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* air_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* ptfe_mat = nist->FindOrBuildMaterial("G4_TEFLON");

  // WALL DIMENSIONS
  G4double wall_sizeXY = fNumberFeaturesSide*(fBaseWidth+fFeaturesSpacing);
  G4double wall_sizeZ = wall_sizeXY/2.;

  // WORLD
  // World is two times bigger than surface
  G4double world_sizeXY = wall_sizeXY*1.5;
  G4double world_sizeZ = wall_sizeZ*1.5;
  // Build the world
  G4Box* solidWorld =    
    new G4Box("World",                                           //name
       world_sizeXY*0.5, world_sizeXY*0.5, world_sizeZ*0.5);     //size   
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,        //solid
                        air_mat,           //material
                        "World");          //name                         
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //logical volume
                      "World",               //name
                      0,                     //mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  // WALL
  G4Box* solidWall =    
    new G4Box("Wall",                                           //name
       wall_sizeXY*0.5, wall_sizeXY*0.5, wall_sizeZ*0.5);                   //size 
  G4LogicalVolume* logicWall =                         
    new G4LogicalVolume(solidWall,        //solid
                        ptfe_mat,           //material
                        "Wall");          //name                              
  G4VPhysicalVolume* physWall = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWall,             //logical volume
                      "Wall",                //name
                      logicWorld,            //mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
  // Move it back, so z=0 is on the surface and not the center of the wall
  G4ThreeVector wallOrigin(0, 0, -wall_sizeZ*0.5);
  physWall->SetTranslation(wallOrigin);


  // PYRAMID
  if(fActivateRoughness)
  {
    G4double shape2_dxa = fBaseWidth, shape2_dxb = 0*cm;
    G4double shape2_dya = fBaseWidth, shape2_dyb = 0*cm;
    G4double shape2_dz  = fFeaturesHeight;      
    G4Trd* solidPyramid =    
      new G4Trd("Shape2",                                         //name
                0.5*shape2_dxa, 0.5*shape2_dxb,                   //size
                0.5*shape2_dya, 0.5*shape2_dyb, 0.5*shape2_dz);   //size
    G4LogicalVolume* logicPyramid =                         
      new G4LogicalVolume(solidPyramid,        //solid
                          ptfe_mat,            //material
                          "Pyramid");          //name   
    G4VPVParameterisation* pyramidParam =
      new ptfe_surfaceParameterisation(fNumberFeaturesSide, //number pyramids
                                       fBaseWidth,          //width of pyramid base
                                       fFeaturesSpacing,     //space between bases
                                       -wall_sizeXY/2.+fBaseWidth/2.,
                                       -wall_sizeXY/2.+fBaseWidth/2.,
                                       wall_sizeZ/2.+fFeaturesHeight/2.);                  //z-location
    new G4PVParameterised("Surface",       // their name
                          logicPyramid,   // their logical volume
                          logicWall,       // Mother logical volume
                          kUndefined,          // Are placed along this axis 
                          fNumberFeaturesSide*fNumberFeaturesSide,    // Number of chambers
                          pyramidParam,    // The parametrisation
                          checkOverlaps); // checking overlaps 
  }

  printf("Detector built.\n");
  return physWorld;
}
