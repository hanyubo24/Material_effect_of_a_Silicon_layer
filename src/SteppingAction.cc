#include "SteppingAction.hh"
#include "DataRecorder.hh"

#include <G4Step.hh>
#include <G4Track.hh>
#include <G4VPhysicalVolume.hh>
#include <G4StepPoint.hh>
#include <G4SystemOfUnits.hh>

SteppingAction::SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step) {

    auto* post = step->GetPostStepPoint();
    if (post->GetStepStatus() != fGeomBoundary) return;
    
    auto* pre = step->GetPreStepPoint();
    auto* prePV = pre->GetPhysicalVolume();
    auto* postPV = post->GetPhysicalVolume();
    
    const G4String preName = prePV ? prePV->GetName() : "";
    const G4String postName = postPV ? postPV->GetName() : "";
    
    const auto* track = step->GetTrack();
    const int trackID = track->GetTrackID();
    const int parent = track->GetParentID();
    if (parent != 0) return; // record primaries only
    
    auto& rec = fMap[trackID];
    rec.pdg = track->GetParticleDefinition()->GetPDGEncoding();
    
    // Entering silicon: world -> SiShellPV
    if (!rec.hasIn && postName == fSiPVName && preName != fSiPVName) {
    const auto p = pre->GetMomentum(); // just before entering, outside silicon
    rec.pxIn = p.x()/MeV; rec.pyIn = p.y()/MeV; rec.pzIn = p.z()/MeV;
    rec.pIn = p.mag()/MeV;
    rec.tIn = post->GetGlobalTime()/ns; // boundary time
    rec.hasIn = true;
    }

    // Exiting silicon: SiShellPV -> world
    if (rec.hasIn && !rec.hasOut && preName == fSiPVName && postName != fSiPVName) {
    const auto p = post->GetMomentum(); // just after exiting, outside silicon
    rec.pxOut = p.x()/MeV; rec.pyOut = p.y()/MeV; rec.pzOut = p.z()/MeV;
    rec.pOut = p.mag()/MeV;
    rec.tOut = post->GetGlobalTime()/ns;
    rec.hasOut = true;
    }
}
