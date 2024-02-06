#ifndef CSIMULATION_H
#define CSIMULATION_H

#include "CFileHandler.h"
#include "SimParser.h"
#include "CUav.h"
#include <list>
#include <unordered_map>

class SimulationManager {
public:
    void init(const CommandsMap& cmds, const ParsedData& params);
    void runSimulation();  

private:
    CommandsMap m_commands = {};
    std::list<CUav> m_uavs = {};
    ParsedData m_simulationParams;
    std::unordered_map<int, std::unique_ptr<CFileHandler>> m_fileHandlers; // Maps UAV number to its file handler with unique pointer
    static double m_currentTime; // Start of simulation time
};

#endif 
