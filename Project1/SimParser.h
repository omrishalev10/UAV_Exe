#ifndef SIMPARSER_H
#define SIMPARSER_H

#include <string>
#include <set>

using namespace std;

// Struct for parsed data from SimParams.ini
struct ParsedData {
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
struct Command {
    double time;
    int uavNumber;
    double x;
    double y;

    // Define the operator < for sorting by time within the set
    bool operator<(const Command& other) const {
        return time < other.time;
    }
};

// Struct to hold a set of commands
struct CommandsSet {
    set<Command> commands;
};

class SimParser {
public:
    static bool readSimParams(const string& filename, ParsedData& data);
    static bool readSimCmds(const string& filename, CommandsSet& cmds);
};

#endif 
