#include "SimParser.h"
#include "SimulationManager.h"
#include <iostream>
#include "Main.h"

using namespace std;

int main() {
    ParsedData data;
    CommandsMap cmds;

    // Assuming the files are named "SimParams.ini" and "SimCmds.txt"
    string simParams = "SimParams.ini";
    string SimCmds = "SimCmds.txt";

    // Read simulation parameters
    if (!SimParser::readSimParams(simParams, data)) {
        cerr << "Failed to read simulation parameters from " << simParams << endl;
        return -1; // Or handle the error as needed
    }

    CheckSimParams(data);

    // Read commands
    if (!SimParser::readSimCmds(SimCmds, cmds)) {
        cerr << "Failed to read commands from " << SimCmds << endl;
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
    // Print the simulation parameters
    cout << "Simulation Parameters:" << endl;
    cout << "Dt: " << data.m_Dt << endl;
    cout << "N_uav: " << data.m_number << endl;
    cout << "Radius: " << data.m_radius << endl;
    cout << "X0: " << data.m_x0 << endl;
    cout << "Y0: " << data.m_y0 << endl;
    cout << "Z0: " << data.m_z0 << endl;
    cout << "Speed0: " << data.m_speed0 << endl;
    cout << "Azimuth: " << data.m_azimuth << endl;
    cout << "TimeLim: " << data.m_timeLim << endl;
    cout << endl; // Add an extra line for better readability
}

void CheckSimCmds(CommandsMap& cmds)
{
    // Print the commands
    cout << "Commands:" << endl;
    for (const auto& cmd : cmds.commands) {
        cout << "Time: " << cmd.first
            << ", UAV Number: " << cmd.second.uavNumber
            << ", X: " << cmd.second.x
            << ", Y: " << cmd.second.y << endl;
    }

}


