#include "SimParser.h"
#include <fstream>
#include <sstream>
#include <vector>

bool SimParser::readSimParams(const string& filename, ParsedData& data)
{
    ifstream file(filename);
    if (!file.is_open()) return false;

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string key;
        if (iss >> key) {
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

bool SimParser::readSimCmds(const string& filename, CommandsSet& cmds)
{
    ifstream file(filename);
    if (!file.is_open()) return false;

    Command cmd;
    while (file >> cmd.time >> cmd.uavNumber >> cmd.x >> cmd.y) {
        cmds.commands.insert(cmd);
    }
    file.close();
    return true;
}
