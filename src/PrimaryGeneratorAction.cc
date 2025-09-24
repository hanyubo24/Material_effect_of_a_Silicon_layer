#include "PrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include <G4Event.hh>
#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>
#include <G4SystemOfUnits.hh>
#include <Randomize.hh>
#include <cmath>

PrimaryGeneratorAction::PrimaryGeneratorAction(const G4String& particleName, G4double pMin, G4double pMax, G4double gunZ )
:fParticleName(particleName), fPMin(pMin), fPMax(pMax), fgunZ(gunZ)
{
     G4int nofParticles = 1;
     fParticleGun = new G4ParticleGun(nofParticles);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
 
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {

  G4double Mdx = G4UniformRand()-0.5; 
  G4double Mdy = G4UniformRand()-0.5;
  G4double Mdz = G4UniformRand()-0.5;

  double cth, phi, p0, theta;
  G4double p;

  p = fPMin + (fPMax - fPMin) * G4UniformRand();

  auto particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle(fParticleName);
  fParticleGun->SetParticleDefinition(particleDefinition);

  G4ThreeVector dir(Mdx, Mdy, Mdz);
  dir = dir.unit(); 

  fParticleGun->SetParticleMomentum(p);
  fParticleGun->SetParticleMomentumDirection(dir);

  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0.,  fgunZ));
  fParticleGun->GeneratePrimaryVertex(event);

}
