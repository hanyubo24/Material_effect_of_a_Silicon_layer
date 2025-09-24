#ifndef DETECTOR_CONSTRUCTION_HH
#define DETECTOR_CONSTRUCTION_HH

#include <G4VUserDetectorConstruction.hh>
#include <G4GenericMessenger.hh>
#include <G4ThreeVector.hh>
#include <G4SystemOfUnits.hh>
#include <memory>

class G4VPhysicalVolume;
class G4LogicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
DetectorConstruction(const std::vector<G4double>& radii, const G4double thickness);
~DetectorConstruction() override = default;

G4VPhysicalVolume* Construct() override;

// Accessors
G4LogicalVolume* GetSiLogical() const { return fSiLV; }

private:
void DefineMaterials();
void SetupMessenger();

// Parameters (changeable via UI)
std::vector<G4double> fRadii;
G4double fThickness; // 250–750 um recommended

// Volumes
G4LogicalVolume* fSiLV = nullptr; // silicon shell LV

std::unique_ptr<G4GenericMessenger> fMessenger;
};

#endif
