#ifndef EVENT_ACTION_HH
#define EVENT_ACTION_HH

#include <G4UserEventAction.hh>

class SteppingAction;

class EventAction : public G4UserEventAction {
public:
    explicit EventAction(SteppingAction* stepper);
    ~EventAction() override = default;
    
    void BeginOfEventAction(const G4Event*) override;
    void EndOfEventAction(const G4Event* evt) override;

private:
    SteppingAction* fStepper; // non-owning
};

#endif
