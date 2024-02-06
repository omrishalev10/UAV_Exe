#ifndef CUAV_H
#define CUAV_H

#include "IUav.h"
#include <utility>
#include <cmath>

static const double glb_PI = 2 * acos(0.0);

// UavParams Struct for UAV-specific parameters
struct UavParams 
{
    int m_uavNumber;
    double m_radius;
    double m_x0;
    double m_y0;
    double m_z0;
    double m_velocity0;
    double m_azimuth;
};

struct UavLocation
{
    double m_x;
    double m_y;
    double m_z;
};

class CUav : public IUav<UavParams> {
public:
    CUav();
    virtual void initialize(const UavParams& params);
    virtual void executeCommand(const Command& cmd) override;
    virtual void update(double deltaTime) override;

private:
    void moveUAV(double deltaTime);
    void calculateAzimuth();
    void calculateSpeedByAxis();
    bool distanceToTarget();
    void moveUavInCircle(double deltaTime);

    int m_uavNumber;
    UavLocation m_currentLocation; // Current position
    double m_velocity;   // Current velocity
    double m_azimuth; // Current azimuth
    double m_turningRadius;
    UavLocation m_destination; // Destination that got from a command
    bool m_isFirstCommand;  // Flag to sign when got THE FIRST command 
    bool m_isNewCommand; // Flag to sign when got new command
    bool isCircling; // Flag to sign when circling
    double m_velocityX; // X Axis speed
    double m_velocityY; // Y Axis speed 
    double m_theta = 0; // Used for circling movement

public:
    /* Getters */
    int getUavNumber() { return m_uavNumber; }
    double getX() { return m_currentLocation.m_x; }
    double getY() { return m_currentLocation.m_y; }
    double getAzimuth() { return m_azimuth; }
};

#endif 