#include "SteppingAction.hh"
#include "SessionManager.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <sstream>

void SteppingAction::UserSteppingAction(const G4Step *step)
{
    SessionManager & SM = SessionManager::getInstance();

    const G4StepPoint * postP  = step->GetPostStepPoint();

    if (postP->GetMaterial() == SM.dummyMat)
    {
        const double time = postP->GetGlobalTime()/ns;
        if (time > SM.TimeLimit) return;

        std::stringstream ss;
        ss.precision(SM.OutputPrecision);

        const G4ThreeVector & pos = postP->GetPosition();
        const G4ThreeVector & dir = postP->GetMomentumDirection();

        ss << step->GetTrack()->GetParticleDefinition()->GetParticleName() << ' ';
        ss << postP->GetKineticEnergy()/keV << ' ';
        ss << time << ' ';
        ss << pos[0]/mm << ' ' << pos[1]/mm << ' ' << pos[2]/mm << ' ';
        ss << dir[0] << ' ' << dir[1] << ' ' << dir[2];

        SM.sendLineToDepoOutput(ss);
    }
}
