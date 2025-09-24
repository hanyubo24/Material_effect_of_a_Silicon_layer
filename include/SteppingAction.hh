#ifndef STEPPING_ACTION_HH
#define STEPPING_ACTION_HH

#include <G4UserSteppingAction.hh>
#include <G4String.hh>
#include <unordered_map>

struct EntryExitRecord {
   bool hasIn = false;
   bool hasOut = false;
   double pxIn=0, pyIn=0, pzIn=0, pIn=0, tIn=0; // MeV/c, ns
   double pxOut=0, pyOut=0, pzOut=0, pOut=0, tOut=0; // MeV/c, ns
   int pdg=0;
};

class SteppingAction : public G4UserSteppingAction {
public:
   SteppingAction();
   ~SteppingAction() override = default;
   void UserSteppingAction(const G4Step* step) override;
   // access to per-track map
   std::unordered_map<int, EntryExitRecord>& GetMap() { return fMap; }
private:
   std::unordered_map<int, EntryExitRecord> fMap; // trackID -> record
   G4String fSiPVName = "Silicon"; // physical volume name
};

#endif
