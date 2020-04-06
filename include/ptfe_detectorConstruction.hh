#ifndef ptfe_detectorConstruction_h
#define ptfe_detectorConstruction_h 1
#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4GenericMessenger;

// Construction class for the detector
class ptfe_detectorConstruction : public G4VUserDetectorConstruction
{
  public:
    ptfe_detectorConstruction();
    virtual ~ptfe_detectorConstruction();

    virtual G4VPhysicalVolume* Construct();

    G4bool ActivateRoughness() const { return fActivateRoughness; }
    G4int NumberFeaturesSide() const { return fNumberFeaturesSide; }
    G4double FeaturesHeight() const { return fFeaturesHeight; }
    G4double BaseWidth() const { return fBaseWidth; }
    G4double FeaturesSpacing() const { return fFeaturesSpacing; }

  private:
    G4GenericMessenger* fMessenger; // Generic messenger class

    G4bool fActivateRoughness; // Whether to simulate roughness
    G4int fNumberFeaturesSide; // Number of pyramids
    G4double fFeaturesHeight; // The height of the features
    G4double fBaseWidth; // The width of the base of the pyramids
    G4double fFeaturesSpacing; // The space between the bases of the pyramids
};

#endif