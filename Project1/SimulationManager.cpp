#include "SimulationManager.h"
#include <chrono>
#include <thread>
#include <exception>
#include <iostream>
#include <iomanip>

SimulationManager::SimulationManager() {}

void SimulationManager::init(const CommandsSet& cmds, const ParsedData& params) {
    this->commands = cmds;
    this->simulationParams = params;

    // Initialize UAVs based on ParsedData
    for (int i = 0; i < params.m_number; ++i) {
        UavParams uavParams{ i+1, params.m_radius, params.m_x0, params.m_y0, params.m_z0, params.m_speed0, params.m_azimuth };
        CUav uav;
        uav.initialize(uavParams); 
        uavs.push_back(uav);
    }
}

void SimulationManager::runSimulation() {
    const double Dt = simulationParams.m_Dt; // Time step interval
    double currentTime = 0.0; // Start of simulation time

    auto nextCmdIt = commands.commands.begin(); // Iterator to the first command

    // Loop until the simulation time reaches the limit
    while (currentTime <= simulationParams.m_timeLim) {
        // Execute commands scheduled for the current time or earlier
        while (nextCmdIt != commands.commands.end() && nextCmdIt->time <= currentTime) {
            for (auto& uav : uavs) {
                if (uav.getUavNumber() == nextCmdIt->uavNumber) {
                    uav.executeCommand(*nextCmdIt);
                }
            }
            ++nextCmdIt; // Move to the next command
        }

        // Update each UAV's state based on Dt
        for (auto& uav : uavs) {
            uav.update(Dt);
        }

        // Optionally, log the state of each UAV at this timestep
        cout << fixed << setprecision(2); // 2 digits after the point
        cout << "Time: " << currentTime << " seconds" << endl;
        for (auto& uav : uavs) {
            cout << "UAV: " << uav.getUavNumber()
                << ", X: " << uav.getX()
                << ", Y: " << uav.getY()
                << ", Azimuth: " << uav.getAzimuth()
                << endl;
        }

        currentTime += Dt; // Advance simulation time by Dt
    }

    // Final log to confirm the end state of each UAV
    cout << "Final UAV States:" << endl;
    for (auto& uav : uavs) {
        cout << "UAV: " << uav.getUavNumber()
            << ", X: " << uav.getX()
            << ", Y: " << uav.getY()
            << ", Azimuth: " << uav.getAzimuth()
            << endl;
    }
}



