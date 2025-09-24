#include "DetectorConstruction.hh"

#include <G4NistManager.hh>
#include <G4Material.hh>
#include <G4Tubs.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4Box.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include <G4RunManager.hh>

DetectorConstruction::DetectorConstruction(const std::vector<G4double>& radii, const G4double thickness): fRadii(radii), fThickness(thickness) {
    DefineMaterials();
    SetupMessenger();
}

void DetectorConstruction::DefineMaterials() {
    auto* nist = G4NistManager::Instance();
    nist->FindOrBuildMaterial("G4_Si");
    nist->FindOrBuildMaterial("G4_Galactic");
}

void DetectorConstruction::SetupMessenger() {
    fMessenger = std::make_unique<G4GenericMessenger>(this, "/det/", "Detector control");
}

G4VPhysicalVolume* DetectorConstruction::Construct() {

    auto* nist = G4NistManager::Instance();
    auto* worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    auto* siliconMaterial = nist->FindOrBuildMaterial("G4_Si");

    G4double angle_f= 12.0*deg;
    G4double angle_b= 26.0*deg; // 180-154*deg

  
    G4double radius_for_testing = fRadii[0]*mm; // so far only use the first layer 
    G4double thickness_here = fThickness * micrometer;
    G4double length_f = radius_for_testing / std::tan(angle_f);
    G4double length_b = radius_for_testing / std::tan(angle_b);
    
    const G4double worldSize_XY = radius_for_testing*2*1.1;
    const G4double worldSize_Z = (length_f+length_b)*2;
 
    
    // World
    auto* solidWorld = new G4Box("World", worldSize_XY/2, worldSize_XY/2, worldSize_Z/2);
    auto* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "WorldLV");
    auto* physWorld = new G4PVPlacement(nullptr, {}, logicWorld, "WorldPV", nullptr, false, 0);
    
    // Silicon shell (G4Tubs: inner R, outer R, half-length)
    auto solidSensor = new G4Tubs("Silicon", radius_for_testing,  radius_for_testing+thickness_here , (length_f+length_b)/2.0, 0.0*deg, 360.0*deg);
    fSiLV = new G4LogicalVolume(solidSensor, siliconMaterial, "Silicon");    
    auto* siliconSensorPV = new G4PVPlacement(0, G4ThreeVector(0,0, length_f - (length_f+length_b)/2.0),fSiLV , "Silicon", logicWorld, false, 0);
    
    // Visualization
    fSiLV->SetVisAttributes(G4VisAttributes(G4Colour::Yellow()));
    logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
    
    return physWorld;
}
