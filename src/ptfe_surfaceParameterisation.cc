#include "ptfe_surfaceParameterisation.hh"

#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4SystemOfUnits.hh"

// Construct/Deconstructor
ptfe_surfaceParameterisation::ptfe_surfaceParameterisation(
  G4int pNumberFeaturesSide,
  G4double pBaseWidth,
  G4double pFeatureSpacing,
  G4double pStartX,
  G4double pStartY,
  G4double pStartZ )
: G4VPVParameterisation()
{
  fNumberFeaturesSide = pNumberFeaturesSide;
  fBaseWidth = pBaseWidth;
  fFeatureSpacing = pFeatureSpacing;
  fStartX = pStartX;
  fStartY = pStartY;
  fStartZ = pStartZ;
}
ptfe_surfaceParameterisation::~ptfe_surfaceParameterisation(){}

// Placement function
void ptfe_surfaceParameterisation::ComputeTransformation
(const G4int pCopyNum, G4VPhysicalVolume* pPhysVol) const
{
  G4int x_grid = (int) pCopyNum/(fNumberFeaturesSide);
  G4int y_grid = (int) pCopyNum%(fNumberFeaturesSide);
  G4double x_loc = x_grid*(fBaseWidth+fFeatureSpacing);
  G4double y_loc = y_grid*(fBaseWidth+fFeatureSpacing);

  G4ThreeVector origin(fStartX+x_loc, fStartY+y_loc, fStartZ);
  pPhysVol->SetTranslation(origin);
  pPhysVol->SetRotation(0);
};