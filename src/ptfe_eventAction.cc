#include "ptfe_eventAction.hh"
#include "g4analysis.hh"

ptfe_eventAction::ptfe_eventAction()
: G4UserEventAction(){}
ptfe_eventAction::~ptfe_eventAction(){}

void ptfe_eventAction::BeginOfEventAction(const G4Event*){}     

void ptfe_eventAction::EndOfEventAction(const G4Event*){}