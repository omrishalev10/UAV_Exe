#ifndef CSIMULATION_H
#define CSIMULATION_H

#include "CFileHandler.h"
#include "SimParser.h"
#include "CUav.h"
#include <set>
#include <list>

using namespace std;

class SimulationManager {
private:
    CommandsSet commands = {};
    list<CUav> uavs = {};
    ParsedData simulationParams;

public:
    SimulationManager();  // Constructor to initialize simulation parameters
    void init(const CommandsSet& cmds, const ParsedData& params);
    void runSimulation();     // Method to run the simulation
};

#endif 
