#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <string>

class G4Material;

class SessionManager
{
    public:
        static SessionManager& getInstance();

    private:
        SessionManager();
        ~SessionManager();

    public:
        SessionManager(SessionManager const&) = delete;
        void operator=(SessionManager const&) = delete;

        void startSession();
        void endSession();

        void runSimulation();

        bool bGuiMode           = false;

        std::string FileName_Output;

        double PMMAlength       = 0;
        G4Material * dummyMat   = nullptr;
        double CylShift         = 0;

        double ProtonEnergy     = 0;
        int numProtonsPerRun    = 0;
        int numRuns             = 0;

        double bunchPeriod      = 0;

        // run time variables
        double bunchTime        = 0;

public:
        void sendLineToDepoOutput(const std::stringstream & text);
        void saveEventNumber(int iEvent);

private:
        std::ofstream * outStreamDeposition = nullptr;
};

#endif // SESSIONMANAGER_H
