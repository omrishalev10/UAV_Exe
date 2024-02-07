#include "SimParser.h"
#include "SimulationManager.h"
#include <iostream>
#include "Main.h"

int main() {
    ParsedData data;
    CommandsMap cmds;

    // Assuming the files are named "SimParams.ini" and "SimCmds.txt"
    std::string simParams = "SimParams.ini";
    std::string SimCmds = "SimCmds.txt";

    // Read simulation parameters
    if (!SimParser::readSimParams(simParams, data)) {
        std::cerr << "Failed to read simulation parameters from " << simParams << std::endl;
        return -1; // Or handle the error as needed
    }
    
    CheckSimParams(data);

    // Read commands
    if (!SimParser::readSimCmds(SimCmds, cmds)) {
        std::cerr << "Failed to read commands from " << SimCmds << std::endl;
        return -1; // Or handle the error as needed
    }

    CheckSimCmds(cmds);

    SimulationManager* simManager = new SimulationManager();
    simManager->init(cmds, data);
    simManager->runSimulation();

    return 0;
}

void CheckSimParams(ParsedData& data)
{
    if (data.m_Dt <= 0)
    {
        std::cout << "You have entered wrong Dt" << std::endl;
        exit(1);
    }
    if (data.m_number <= 0)
    {
        std::cout << "You have entered wrong N_Uav" << std::endl;
        exit(1);
    }
    if (data.m_speed0 <= 0) {
        std::cout << "You have entered wrong V0" << std::endl;
        exit(1);
    }
    if (data.m_timeLim <= 0) {
        std::cout << "You have entered wrong timeLim" << std::endl;
        exit(1);
    }
    // Print the simulation parameters
    std::cout << "Simulation Parameters:" << std::endl;
    std::cout << "Dt: " << data.m_Dt << std::endl;
    std::cout << "N_uav: " << data.m_number << std::endl;
    std::cout << "Radius: " << data.m_radius << std::endl;
    std::cout << "X0: " << data.m_x0 << std::endl;
    std::cout << "Y0: " << data.m_y0 << std::endl;
    std::cout << "Z0: " << data.m_z0 << std::endl;
    std::cout << "Speed0: " << data.m_speed0 << std::endl;
    std::cout << "Azimuth: " << data.m_azimuth << std::endl;
    std::cout << "TimeLim: " << data.m_timeLim << std::endl;
    std::cout << std::endl; 
}

void CheckSimCmds(CommandsMap& cmds)
{
    
    for (const auto& cmdPair : cmds.m_commands)
    {
        if (cmdPair.first < 0)
        {
            std::cout << "You have entered wrong time" << std::endl;
            exit(1);
        }
        if (cmdPair.second.m_uavNumber <= 0)
        {
            std::cout << "You have entered wrong uavNumber" << std::endl;
            exit(1);
        }      
    }
    
    // Print the commands
    std::cout << "Commands:" << std::endl;
    for (const auto& cmd : cmds.m_commands) {
        std::cout << "Time: " << cmd.first
            << ", UAV Number: " << cmd.second.m_uavNumber
            << ", X: " << cmd.second.m_x
            << ", Y: " << cmd.second.m_y << std::endl;
    }
}