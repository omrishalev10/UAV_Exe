#ifndef CSIMULATION_H
#define CSIMULATION_H

#include "CFileHandler.h"
#include "SimParser.h"
#include "CUav.h"
#include <set>
#include <list>
#include <unordered_map>

using namespace std;

class SimulationManager {
private:
    CommandsSet commands = {};
    list<CUav> uavs = {};
    ParsedData simulationParams;
    unordered_map<int, unique_ptr<CFileHandler>> fileHandlers; // Maps UAV number to its file handler with unique pointer
    static double currentTime; // Start of simulation time

public:
    SimulationManager();  
    void init(const CommandsSet& cmds, const ParsedData& params);
    void runSimulation();     
    ~SimulationManager(); 
};

#endif 
