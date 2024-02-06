#ifndef SIMPARSER_H
#define SIMPARSER_H

#include <string>
#include <map>

// Struct for parsed data from SimParams.ini
struct ParsedData 
{
    double m_Dt;
    int m_number;
    double m_radius;
    double m_x0;
    double m_y0;
    double m_z0;
    double m_speed0;
    double m_azimuth;
    double m_timeLim;
};

// Struct for individual command
struct Command 
{
    double m_time;
    int m_uavNumber;
    double m_x;
    double m_y;

    // Define the operator < for sorting by time within the set
    bool operator<(const Command& other) const 
    {
        return m_time < other.m_time;
    }
};

// Struct to hold a set of commands
struct CommandsMap 
{
    std::multimap<double, Command> m_commands;
};

class SimParser 
{
public:
    static bool readSimParams(const std::string& filename, ParsedData& data);
    static bool readSimCmds(const std::string& filename, CommandsMap& cmds);
};

#endif 
