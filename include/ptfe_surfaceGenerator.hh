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
    std::vector<G4double> GetHeightVector(){ return frOuter; }
    G4double InterpoleHeight(G4double x);

  private:
  G4int fNumSide;
  G4int fNumZPlanes;
  G4double fMinY;
  G4double fMaxY;
  G4double fMinX;
  G4double fMaxX;
  G4double fLength;
  std::vector<G4double> frOuter, fzPlane;
};

#endif

