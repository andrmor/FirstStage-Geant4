#include "PrimaryGeneratorAction.hh"
#include "SessionManager.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction()
{
    SessionManager & SM = SessionManager::getInstance();
    G4int nofParticles = SM.numProtonsPerRun;
    fParticleGun = new G4ParticleGun(nofParticles);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    fParticleGun->SetParticleDefinition(particleTable->FindParticle("proton"));

    fParticleGun->SetParticlePosition(G4ThreeVector(-10.0*mm, 0, 0));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.0, 0, 0));
    fParticleGun->SetParticleEnergy(SM.ProtonEnergy*MeV);
    fParticleGun->SetParticleTime(0*ns);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    SessionManager & SM = SessionManager::getInstance();

    fParticleGun->SetParticleTime(SM.bunchTime*ns);
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
