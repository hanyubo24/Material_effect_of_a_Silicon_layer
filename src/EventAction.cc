#include "EventAction.hh"
#include "SteppingAction.hh"
#include "DataRecorder.hh"

#include <G4Event.hh>

EventAction::EventAction(SteppingAction* stepper) : fStepper(stepper) {}

void EventAction::BeginOfEventAction(const G4Event*) {
// nothing
}

void EventAction::EndOfEventAction(const G4Event* evt) {
auto& mp = fStepper->GetMap();
const int eventID = evt->GetEventID();

for (auto it = mp.begin(); it != mp.end(); ) {
const int trackID = it->first;
const auto& rec = it->second;

if (rec.hasIn && rec.hasOut) {
DataRecorder::Instance().WriteRow(eventID, trackID, rec.pdg,
rec.pxIn, rec.pyIn, rec.pzIn, rec.pIn, rec.tIn,
rec.pxOut, rec.pyOut, rec.pzOut, rec.pOut, rec.tOut);
}
it = mp.erase(it); // clear per-event
}
}
