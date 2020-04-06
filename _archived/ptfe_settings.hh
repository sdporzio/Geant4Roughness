#ifndef ptfe_settings_h
#define ptfe_settings_h 1

#include "globals.hh"

class ptfe_settings
{
  public:
    ptfe_settings();
    virtual ~ptfe_settings();

    G4GenericMessenger* fMessenger; // Generic messenger class

    G4bool fActivateRoughness; // Whether to simulate roughness
    G4int fNumberFeaturesSide; // Number of pyramids
    G4double fFeaturesHeight; // The height of the features
    G4double fBaseWidth; // The width of the base of the pyramids
    G4double fFeaturesSpacing; // The space between the bases of the pyramids
};

#endif