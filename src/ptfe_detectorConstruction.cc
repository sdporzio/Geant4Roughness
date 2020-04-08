#include "ptfe_detectorConstruction.hh"
#include "ptfe_surfaceParameterisation.hh"

#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"

#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4NistManager.hh"
#include "G4Element.hh"
#include "G4Isotope.hh"
#include "G4Material.hh"


#include "g4analysis.hh"

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
  // HARD-CODED SETTINGS
  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = false;

  // NIST MATERIALS
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* air_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* nist_water = nist->FindOrBuildMaterial("G4_WATER");
  G4Material* nist_teflon = nist->FindOrBuildMaterial("G4_TEFLON");

  // BUILD LIQUID XENON
  G4double xeTemperature = 173 * kelvin;
  G4double xePressure = 1.57 * bar;
  G4double xeDensity = 2.888 * g / cm3;

  G4Isotope* Xe124 = new G4Isotope("Xe124", 54, 124, 123.9058942 * g / mole);
  G4Isotope* Xe126 = new G4Isotope("Xe126", 54, 126, 125.904281 * g / mole);
  G4Isotope* Xe128 = new G4Isotope("Xe128", 54, 128, 127.9035312 * g / mole);
  G4Isotope* Xe129 = new G4Isotope("Xe129", 54, 129, 128.9047801 * g / mole);
  G4Isotope* Xe130 = new G4Isotope("Xe130", 54, 130, 129.9035094 * g / mole);
  G4Isotope* Xe131 = new G4Isotope("Xe131", 54, 131, 130.905072 * g / mole);
  G4Isotope* Xe132 = new G4Isotope("Xe132", 54, 132, 131.904144 * g / mole);
  G4Isotope* Xe134 = new G4Isotope("Xe134", 54, 134, 133.905395 * g / mole);
  G4Isotope* Xe136 = new G4Isotope("Xe136", 54, 136, 135.907214 * g / mole);

  G4Element* natXe = new G4Element("Natural Xe", "natXe", 9);
  natXe->AddIsotope(Xe124, 0.09 * perCent);
  natXe->AddIsotope(Xe126, 0.09 * perCent);
  natXe->AddIsotope(Xe128, 1.92 * perCent);
  natXe->AddIsotope(Xe129, 26.44 * perCent);
  natXe->AddIsotope(Xe130, 4.08 * perCent);
  natXe->AddIsotope(Xe131, 21.18 * perCent);
  natXe->AddIsotope(Xe132, 26.89 * perCent);
  natXe->AddIsotope(Xe134, 10.44 * perCent);
  natXe->AddIsotope(Xe136, 8.87 * perCent);

  G4Material* liquidXe = new G4Material("liquidXe", xeDensity, 1, kStateLiquid,
    xeTemperature, xePressure);
  liquidXe->AddElement(natXe, 1);

  // BUILD TEFLON
  G4Isotope* C12 = new G4Isotope("C12", 6, 12, 12. * g / mole);
  G4Isotope* C13 = new G4Isotope("C13", 6, 13, 13.003354826 * g / mole);
  G4Isotope* F19 = new G4Isotope("F19", 9, 19, 18.99840322 * g / mole);

  G4Element* natC = new G4Element("Natural C", "natC", 2);
    natC->AddIsotope(C12, 98.93 * perCent);
    natC->AddIsotope(C13, 1.07 * perCent);

  G4Element* natF = new G4Element("Natural F", "natF", 1);
    natF->AddIsotope(F19, 100. * perCent);

  G4Material* teflon = new G4Material("teflon", 2.16 * g / cm3, 2);
    teflon->AddElement(natC, 1);
    teflon->AddElement(natF, 2);


  // WALL DIMENSIONS
  G4double wall_sizeXY = fNumberFeaturesSide*(fBaseWidth+fFeaturesSpacing);
  G4double wall_sizeZ = wall_sizeXY/2.;

  // WORLD
  // World is two times bigger than surface
  G4double world_sizeXY = wall_sizeXY*20;
  G4double world_sizeZ = wall_sizeZ*20;
  // Build the world
  G4Box* solidWorld =    
    new G4Box("World",                                           //name
       world_sizeXY*0.5, world_sizeXY*0.5, world_sizeZ*0.5);     //size   
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,        //solid
                        nist_water,           //material
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
  // Give it a sensible colour
  G4VisAttributes* worldColour= new G4VisAttributes(G4Colour(0.2,0.2,0.9,0.1));
  logicWorld->SetVisAttributes(worldColour);

  // WALL
  G4Box* solidWall =    
    new G4Box("Wall",                                           //name
       wall_sizeXY*0.5, wall_sizeXY*0.5, wall_sizeZ*0.5);       //size 
  G4LogicalVolume* logicWall =                         
    new G4LogicalVolume(solidWall,        //solid
                        nist_teflon,         //material
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
  // Give it a sensible colour
  G4VisAttributes* wallColour= new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.75));
  logicWall->SetVisAttributes(wallColour);

  // PYRAMID
  if(fActivateRoughness)
  {
    // SURFACE CUSHION
    // Mother volume for the surface
    G4Box* solidCushion =    
      new G4Box("Cushion",                                           //name
         wall_sizeXY*0.5, wall_sizeXY*0.5, fFeaturesHeight*0.5);     //size 
    G4LogicalVolume* logicCushion =                         
      new G4LogicalVolume(solidCushion,    //solid
                          liquidXe,        //material
                          "Cushion");      //name     
    G4VPhysicalVolume* physCushion = 
      new G4PVPlacement(0,                     //no rotation
                        G4ThreeVector(),       //at (0,0,0)
                        logicCushion,             //logical volume
                        "Wall",                //name
                        logicWorld,            //mother  volume
                        false,                 //no boolean operation
                        0,                     //copy number
                        checkOverlaps);        //overlaps checking
    // Move it back, so z=0 is on the surface and not the center of the wall
    G4ThreeVector cushionOrigin(0, 0, fFeaturesHeight*0.5);
    physCushion->SetTranslation(cushionOrigin);
    // Give it a sensible colour
    G4VisAttributes* cushionColour= new G4VisAttributes(G4Colour(0.5,0.5,1,0.1));
    logicCushion->SetVisAttributes(cushionColour);

    //ROUGH SURFACE
    G4double shape2_dxa = fBaseWidth, shape2_dxb = 0*cm;
    G4double shape2_dya = fBaseWidth, shape2_dyb = 0*cm;
    G4double shape2_dz  = fFeaturesHeight;      
    G4Trd* solidPyramid =    
      new G4Trd("RoughSurface",                                   //name
                0.5*shape2_dxa, 0.5*shape2_dxb,                   //size
                0.5*shape2_dya, 0.5*shape2_dyb, 0.5*shape2_dz);   //size
    G4LogicalVolume* logicPyramid =                         
      new G4LogicalVolume(solidPyramid,        //solid
                          teflon,            //material
                          "RoughSurface");          //name   
    G4VPVParameterisation* pyramidParam =
      new ptfe_surfaceParameterisation(fNumberFeaturesSide, //number pyramids
                                       fBaseWidth,          //width of pyramid base
                                       fFeaturesSpacing,     //space between bases
                                       -wall_sizeXY/2.+fBaseWidth/2.,
                                       -wall_sizeXY/2.+fBaseWidth/2.,
                                       0);                  //z-location
    new G4PVParameterised("RoughSurface",       // their name
                          logicPyramid,   // their logical volume
                          logicCushion,       // Mother logical volume
                          kUndefined,          // Are placed along this axis 
                          fNumberFeaturesSide*fNumberFeaturesSide,    // Number of chambers
                          pyramidParam,    // The parametrisation
                          checkOverlaps); // checking overlaps 
    // Give it a sensible colour
    G4VisAttributes* spikesColour= new G4VisAttributes(G4Colour(0.9,0.9,0.9,1));
    logicPyramid->SetVisAttributes(spikesColour);
  } // END IF rough surface will be drawn


  return physWorld;
}
