#include "SessionManager.hh"

#include <iostream>
#include <sstream>
#include <fstream>

#include "G4ParticleDefinition.hh"
#include "G4UImanager.hh"

SessionManager &SessionManager::getInstance()
{
    static SessionManager instance; // Guaranteed to be destroyed, instantiated on first use.
    return instance;
}

SessionManager::SessionManager(){}

SessionManager::~SessionManager()
{
    delete outStreamDeposition;
}

void SessionManager::startSession()
{
    outStreamDeposition = new std::ofstream();
    outStreamDeposition->open(FileName_Output);
    if (!outStreamDeposition->is_open())
    {
        std::cerr << "Cannot open file to store generated particles" << std::endl;
        exit(1);
    }
}

void SessionManager::endSession()
{
    outStreamDeposition->close();
}

void SessionManager::runSimulation()
{
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    for (int i=0; i<numRuns; i++)
    {
        if (i % 100 == 0) std::cout << "Run # " << i << "  bunch time: " << bunchTime << std::endl;
        saveEventNumber(i);

        UImanager->ApplyCommand("/run/beamOn");

        bunchTime += bunchPeriod;
    }
}

void SessionManager::sendLineToDepoOutput(const std::stringstream & text)
{
    *outStreamDeposition << text.rdbuf() << std::endl;
}

void SessionManager::saveEventNumber(int iEvent)
{
    std::stringstream ss;
    ss << '#' << iEvent;
    *outStreamDeposition << ss.rdbuf() << std::endl;
}
