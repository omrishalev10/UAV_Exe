#include "SimParser.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

bool SimParser::readSimParams(const std::string& filename, ParsedData& data)
{
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line;
    while (getline(file, line))
    {
        std::istringstream iss(line);
        std::string key;
        char eq;
        if (iss >> key >> eq) 
        {
            if (key == "Dt") iss >> data.m_Dt;
            else if (key == "N_uav") iss >> data.m_number;
            else if (key == "R") iss >> data.m_radius;
            else if (key == "X0") iss >> data.m_x0;
            else if (key == "Y0") iss >> data.m_y0;
            else if (key == "Z0") iss >> data.m_z0;
            else if (key == "V0") iss >> data.m_speed0;
            else if (key == "Az") iss >> data.m_azimuth;
            else if (key == "TimeLim") iss >> data.m_timeLim;
        }       
    }    
    file.close();
    return true;
}

bool SimParser::readSimCmds(const std::string& filename, CommandsMap& cmds)
{
    std::ifstream file(filename);
    if (!file.is_open()) 
    {
        return false;
    }
    Command cmd = { 0 };
    while (file >> cmd.m_time >> cmd.m_uavNumber >> cmd.m_x >> cmd.m_y)
    {
        // Insert command into multimap with time as the key
        cmds.m_commands.insert(std::make_pair(cmd.m_time, cmd));
    }
    file.close();
    return true;
}