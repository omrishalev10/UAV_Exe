#include "SimulationManager.h"
#include <iostream>
#include <sstream>
#include <iomanip> // setprecision function

double SimulationManager::currentTime = 0.0;

SimulationManager::SimulationManager() {}

void SimulationManager::init(const CommandsSet& cmds, const ParsedData& params) {
    this->commands = cmds;
    this->simulationParams = params;

    /* Initialize UAVs based on ParsedData */
    for (int i = 0; i < params.m_number; ++i) {
        UavParams uavParams{ i + 1, params.m_radius, params.m_x0, params.m_y0, params.m_z0, params.m_speed0, params.m_azimuth };
        CUav uav;
        uav.initialize(uavParams);
        uavs.push_back(uav);

        /* Create a file for each UAV */
        ostringstream fileName;
        fileName << "UAV" << i + 1 << ".txt";
        auto handler = make_unique<CFileHandler>(); // Create a unique_ptr to CFileHandler
        if (!handler->initHandler(fileName.str())) {
            cerr << "Failed to initialize file handler for UAV" << (i + 1) << endl;
        }
        fileHandlers.emplace(i + 1, move(handler)); // Store the unique_ptr in the map
     }
}

void SimulationManager::runSimulation() {
    const double Dt = simulationParams.m_Dt; // Time step interval
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
        for (auto& uav : uavs) 
        {
            if (currentTime == 0.0)
            {
                uav.update(currentTime);
            }
            else
            {
                uav.update(Dt);
            }
        }

        for (auto& uav : uavs) {
            ostringstream data;
            /* This is the initialize states of the uav */
            /*if (currentTime == 0) {
                data << fixed << setprecision(2)
                    << "Time: " << currentTime << ", UAV: " << uav.getUavNumber()
                    << ", X: " << simulationParams.m_x0 << ", Y: " << simulationParams.m_y0
                    << ", Azimuth: " << simulationParams.m_azimuth << endl;
            }*/
            /* else {*/
            data << fixed << setprecision(2)
                << "Time: " << currentTime << ", UAV: " << uav.getUavNumber()
                << ", X: " << uav.getX() << ", Y: " << uav.getY()
                << ", Azimuth: " << uav.getAzimuth() << endl;
            //}
            // Write to the corresponding file
            fileHandlers[uav.getUavNumber()]->writeHandler(data.str());
        }

        currentTime += Dt;
    }
}