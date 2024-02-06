#include "SimulationManager.h"
#include <iostream>
#include <sstream>
#include <iomanip> // setprecision function

double SimulationManager::m_currentTime = 0.0;

void SimulationManager::init(const CommandsMap& cmds, const ParsedData& params) {
    this->m_commands = cmds;
    this->m_simulationParams = params;

    /* Initialize UAVs based on ParsedData */
    for (int i = 0; i < params.m_number; ++i) 
    {
        UavParams uavParams{ i + 1, params.m_radius, params.m_x0, params.m_y0, params.m_z0, params.m_speed0, params.m_azimuth };
        CUav uav;
        uav.initialize(uavParams);
        m_uavs.push_back(uav);

        /* Create a file for each UAV */
        std::ostringstream fileName;
        fileName << "UAV" << i + 1 << ".txt";
        auto handler = std::make_unique<CFileHandler>(); // Create a unique_ptr to CFileHandler
        if (!handler->initHandler(fileName.str())) 
        {
            std::cerr << "Failed to initialize file handler for UAV" << (i + 1) << std::endl;
        }
        m_fileHandlers.emplace(i + 1, move(handler)); // Store the unique_ptr in the map
     }
}

void SimulationManager::runSimulation() 
{
    const double Dt = m_simulationParams.m_Dt; // Time step interval
    auto nextCmdIt = m_commands.m_commands.begin(); // Iterator to the first command

    // Loop until the simulation time reaches the limit
    while (m_currentTime <= m_simulationParams.m_timeLim) 
    {
        // Update each UAV's state based on Dt
        for (auto& uav : m_uavs)
        {
            if (m_currentTime == 0.0)
            {
                uav.update(m_currentTime);
            }
            else
            {
                uav.update(Dt);
            }
        }
        for (auto& uav : m_uavs)
        {
            std::ostringstream data;
            data << std::fixed << std::setprecision(2)
            << "Time: " << m_currentTime << ", UAV: " << uav.getUavNumber()
            << ", X: " << uav.getX() << ", Y: " << uav.getY()
            << ", Azimuth: " << uav.getAzimuth() << std::endl;
           
            // Write to the corresponding file
            m_fileHandlers[uav.getUavNumber()]->writeHandler(data.str());
        }
        // Execute commands scheduled for the current time or earlier
        while (nextCmdIt != m_commands.m_commands.end() && nextCmdIt->first <= m_currentTime) 
        {
            for (auto& uav : m_uavs) 
            {
                if (uav.getUavNumber() == nextCmdIt->second.m_uavNumber) 
                {
                    uav.executeCommand(nextCmdIt->second);
                }
            }
            ++nextCmdIt; // Move to the next command
        }
        m_currentTime += Dt;
    }
    for (auto& fileHandler : m_fileHandlers)
    {
        fileHandler.second->closeHandler();
    }
}