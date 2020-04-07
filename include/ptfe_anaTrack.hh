#ifndef ptfe_anaTrack_h
#define ptfe_anaTrack_h 1

#include "globals.hh"


class ptfe_anaTrack
{
  public:
    ptfe_anaTrack();
    virtual ~ptfe_anaTrack();

    G4String name;
    G4int pdg;
    G4int trackId;
    G4int parentId;
    G4double xStart, yStart, zStart;
    G4double pxStart, pyStart, pzStart;
    G4double pStart, eStart, tStart;
    G4double xEnd, yEnd, zEnd;
    G4double pxEnd, pyEnd, pzEnd;
    G4double pEnd, eEnd, tEnd;
    G4String volumeStart, volumeEnd;

};

#endif
