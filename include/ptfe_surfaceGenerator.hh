#ifndef ptfe_surfaceGenerator_h
#define ptfe_surfaceGenerator_h 1
#include "G4VSolid.hh"

// Construction class for parameterised surface with repeated elements
class ptfe_surfaceGenerator
{
  public:
    ptfe_surfaceGenerator();
    virtual ~ptfe_surfaceGenerator();

    virtual G4VSolid* GenerateSurfaceSolid();

  private:
    G4int fNumberFeaturesSide;
    G4bool fActivateRoughness;
    G4double fWallWidth;
    G4double fWallDepth;
    G4double fFeaturesHeight;
    G4double fBaseWidth;
    G4double fFeaturesSpacing;
};

#endif