#include "ptfe_surfaceGenerator.hh"
#include "ptfe_detectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4MultiUnion.hh"
#include "G4UnionSolid.hh"

// Construct/Deconstructor
// Construct a G4Solid which corresponds to a rough surface, by merging 
// together a cube (wall) with a series of piramids solids (surface)
ptfe_surfaceGenerator::ptfe_surfaceGenerator()
{
  // Get the detector (for metadata purposes)
  const ptfe_detectorConstruction* myDetector
      = static_cast<const ptfe_detectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  fActivateRoughness = myDetector->ActivateRoughness();
  fWallWidth = myDetector->WallWidth();
  fWallDepth = myDetector->WallDepth();
  fFeaturesHeight = myDetector->FeaturesHeight();
  fBaseWidth = myDetector->BaseWidth();
  fFeaturesSpacing = myDetector->FeaturesSpacing();
}
ptfe_surfaceGenerator::~ptfe_surfaceGenerator()
{}

G4VSolid* ptfe_surfaceGenerator::GenerateSurfaceSolid()
{

  // WALL 
  // Create support wall
  printf("Input wall dimensions: %.1f x %.1f x %.1f um\n", fWallWidth/um, fWallWidth/um,fWallDepth/um);
  printf("Input feature dimensions: %.1f (h) x %.1f (b) um with %.1f nm spacing\n", fFeaturesHeight/um, fBaseWidth/um, fFeaturesSpacing/nm);

  G4double wall_sizeXY, wall_sizeZ;
  if(fActivateRoughness)
  {
    fNumberFeaturesSide = int(fWallWidth/(fBaseWidth+fFeaturesSpacing))+1;
    printf("Number features produced: %i\n", fNumberFeaturesSide*fNumberFeaturesSide);
    wall_sizeXY = fNumberFeaturesSide*(fBaseWidth+fFeaturesSpacing);
    wall_sizeZ = fWallDepth;
    printf("Actuall wall dimensions: %.1f x %.1f x %.1f um\n", wall_sizeXY/um, wall_sizeXY/um,wall_sizeZ/um);
  }
  else
  {
    wall_sizeXY = fWallWidth;
    wall_sizeZ = fWallDepth;
  }

  G4Box* solidWall =    
    new G4Box("Wall",                                           //name
       wall_sizeXY*.5, wall_sizeXY*.5, wall_sizeZ*.5);       //size


  if(fActivateRoughness)
  {
    // PYRAMID
    // Create pyramid solid
    G4double shape2_dxa = fBaseWidth, shape2_dxb = 0*cm;
    G4double shape2_dya = fBaseWidth, shape2_dyb = 0*cm;
    G4double shape2_dz  = fFeaturesHeight;      
    G4Trd* solidPyramid =    
      new G4Trd("Pyramid",                                   //name
                0.5*shape2_dxa, 0.5*shape2_dxb,                   //size
                0.5*shape2_dya, 0.5*shape2_dyb, 0.5*shape2_dz);   //size


    // Join the solids together
    G4int totalNumberFeatures = fNumberFeaturesSide*fNumberFeaturesSide;
    G4double originXY = -wall_sizeXY*.5 + fBaseWidth*.5;
    G4double originZ = wall_sizeZ*.5 + fFeaturesHeight*.5;
    G4MultiUnion* unionSolid = new G4MultiUnion("RoughSurface");
    G4Transform3D tr1 = G4Transform3D(G4RotationMatrix(),G4ThreeVector());
    unionSolid->AddNode(*solidWall,tr1);
    for(int i=0;i<totalNumberFeatures;i++)
    {

      G4int x_grid = (int) i/(fNumberFeaturesSide);
      G4int y_grid = (int) i%(fNumberFeaturesSide);
      G4double x_loc = x_grid*(fBaseWidth+fFeaturesSpacing);
      G4double y_loc = y_grid*(fBaseWidth+fFeaturesSpacing);

      G4Transform3D tr2 = G4Transform3D(G4RotationMatrix(),G4ThreeVector(originXY+x_loc,originXY+y_loc,originZ));
      unionSolid->AddNode(*solidPyramid,tr2);
    }
    unionSolid->Voxelize();
    return unionSolid;
  }
  else
  {
    return solidWall;
  }

}


