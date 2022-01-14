#include "SessionManager.hh"

#include <iostream>
#include <sstream>
#include <fstream>

#include "G4ParticleDefinition.hh"
#include "G4UImanager.hh"
#include "G4String.hh"

SessionManager &SessionManager::getInstance()
{
    static SessionManager instance; // Guaranteed to be destroyed, instantiated on first use.
    return instance;
}

SessionManager::SessionManager(){}

SessionManager::~SessionManager()
{
    delete outStream;
}

void SessionManager::startSession()
{
    outStream = new std::ofstream();

    if (bBinaryOutput) outStream->open(FileName_Output, std::ios::out | std::ios::binary);
    else               outStream->open(FileName_Output);

    if (!outStream->is_open())
    {
        std::cerr << "Cannot open file to store generated particles" << std::endl;
        exit(1);
    }
}

void SessionManager::endSession()
{
    outStream->close();
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

void SessionManager::saveEventNumber(int iEvent)
{
    if (bBinaryOutput)
    {
        *outStream << char(0xee);
        outStream->write((char*)&iEvent, sizeof(int));
    }
    else
    {
        *outStream << '#' << iEvent << '\n';
        //std::stringstream ss;
        //ss << '#' << iEvent;
        //*outStream << ss.rdbuf() << std::endl;
    }
}


void SessionManager::saveParticle(const G4String & particle, double energy, double time, double * PosDir)
{
    if (bBinaryOutput)
    {
        // 0xFF(char) ParticleName(string) 0x00(char) Energy(double)[keV] X(double)[mm] Y(double)[mm] Z(double)[mm] DirX(double) DirY(double) DirZ(double) Time(double)[ns]

        *outStream << char(0xff);
        *outStream << particle << char(0x00);
        outStream->write((char*)&energy, sizeof(double));
        outStream->write((char*)PosDir, 6*sizeof(double));
        outStream->write((char*)&time, sizeof(double));
    }
    else
    {
        // ParticleName Energy[keV] X[mm] Y[mm] Z[mm] DirX DirY DirZ Time[ns]

        std::stringstream ss;
        ss.precision(OutputPrecision);

        ss << particle << ' ';
        ss << energy << ' ';
        ss << PosDir[0] << ' ' << PosDir[1] << ' ' << PosDir[2] << ' ';     //position
        ss << PosDir[3] << ' ' << PosDir[4] << ' ' << PosDir[5] << ' ';     //direction
        ss << time;

        *outStream << ss.rdbuf() << '\n';
    }
}
