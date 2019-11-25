#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <string>

class G4Material;
class G4String;

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
        bool bBinaryFile        = false;

        double PMMAlength       = 0;
        G4Material * dummyMat   = nullptr;
        double CylShift         = 0;

        double ProtonEnergy     = 0;
        int numProtonsPerRun    = 0;
        int numRuns             = 0;

        double bunchPeriod      = 0;

        // run time variables
        double bunchTime        = 0;

        double TimeLimit        = 0;
        int    OutputPrecision  = 6;


public:
        void saveParticle(const G4String & particle, double energy, double time, double *PosDir);
        void saveEventNumber(int iEvent);

private:
        std::ofstream * outStream = nullptr;
};

#endif // SESSIONMANAGER_H
