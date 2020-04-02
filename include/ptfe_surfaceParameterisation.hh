#ifndef ptfe_surfaceParameterisation_h
#define ptfe_surfaceParameterisation_h 1
#include "G4VPVParameterisation.hh"

class G4VPhysicalVolume;

// Construction class for parameterised surface with repeated elements
class ptfe_surfaceParameterisation : public G4VPVParameterisation
{
  public:
    ptfe_surfaceParameterisation(
      G4int pNumberFeaturesSide,
      G4double pBaseWidth,
      G4double pFeatureSpacing,
      G4double pStartX,
      G4double pStartY,
      G4double pStartZ );
    virtual ~ptfe_surfaceParameterisation();

    void ComputeTransformation(const G4int pCopyNum, G4VPhysicalVolume* pPhysVol) const;

  private:
    G4int fNumberFeaturesSide;
    G4double fBaseWidth, fFeatureSpacing, fStartX, fStartY, fStartZ;
};

#endif