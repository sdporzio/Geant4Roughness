#ifndef ptfe_surfaceGenerator_h
#define ptfe_surfaceGenerator_h 1
#include "G4VSolid.hh"

// Construction class for parameterised surface with repeated elements
class ptfe_surfaceGenerator
{
  public:
    ptfe_surfaceGenerator();
    virtual ~ptfe_surfaceGenerator();

    virtual G4VSolid* GenerateSurfaceSolid_1();
    virtual G4VSolid* GenerateSurfaceSolid_2();
    virtual G4VSolid* GenerateSurfaceSolid_3();
    G4double GetMinY(){ return fMinY; }
    G4double GetMaxY(){ return fMaxY; }
    G4double GetMinX(){ return fMinX; }
    G4double GetMaxX(){ return fMaxX; }
    G4double GetLength(){ return fLength; }

  private:
  G4int fNumSide;
  G4int fNumZPlanes;
  G4double fMinY;
  G4double fMaxY;
  G4double fMinX;
  G4double fMaxX;
  G4double fLength;
};

#endif

