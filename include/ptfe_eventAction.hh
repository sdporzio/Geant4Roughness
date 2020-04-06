#ifndef ptfe_eventAction_h
#define ptfe_eventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4Event;

class ptfe_eventAction : public G4UserEventAction
{
public:
    ptfe_eventAction();
    virtual ~ptfe_eventAction();
    
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
};

#endif
