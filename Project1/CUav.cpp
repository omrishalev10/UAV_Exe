#include "CUav.h"
#include <iostream>
#include <sstream>
#include <iomanip> // setprecision function

CUav::CUav() : m_currentLocation({ 0.0, 0.0, 0.0 }), m_velocity(0), m_azimuth(0), m_uavNumber(0), m_turningRadius(0), m_destination({ 0.0, 0.0 }),
m_isNewCommand(false), m_isFirstCommand(false), isCircling(false), m_velocityX(0), m_velocityY(0) {}

void CUav::initialize(const UavParams& params) 
{
    this->m_uavNumber = params.m_uavNumber;
    this->m_turningRadius = params.m_radius;
    this->m_currentLocation.m_x = params.m_x0;
    this->m_currentLocation.m_y = params.m_y0;
    this->m_currentLocation.m_z = params.m_z0;  
    this->m_velocity = params.m_velocity0;
    this->m_azimuth = params.m_azimuth;
}

void CUav::executeCommand(const Command& cmd) 
{
    /* Make sure we are working with the correct UAV */
    if (cmd.m_uavNumber == m_uavNumber) 
    {
        /* Set new destination */
        m_destination.m_x = cmd.m_x;
        m_destination.m_y = cmd.m_y;         
        if (!m_isNewCommand) 
        { 
            m_isNewCommand = true;
        }
    }
}

void CUav::update(double deltaTime) 
{
    /* Calculate new azimuth only if there was a new command */
    if (m_isNewCommand) 
    {
        if (isCircling)
        {
            isCircling = false;
        }
        calculateAzimuth();
        m_isNewCommand = false;
        m_isFirstCommand = true;
    }
    moveUAV(deltaTime);
}

void CUav::moveUAV(double deltaTime) 
{ 
    // Before the first command uav cant circling.
    if (!m_isFirstCommand) 
    {
        calculateSpeedByAxis();
        m_currentLocation.m_x += m_velocityX * deltaTime;
        m_currentLocation.m_y += m_velocityY * deltaTime;
    }
    else 
    {
        isCircling = true;
        moveUavInCircle(deltaTime);
    }
}

void CUav::calculateAzimuth() 
{
    /* Calculation of delta x and delta y */
    double deltaX = m_destination.m_x - m_currentLocation.m_x;
    double deltaY = m_destination.m_y - m_currentLocation.m_y;

    /* Calculation of the azimuth in radians and turn into degrees */
    m_azimuth = atan2(deltaY, deltaX) * (180.0 / glb_PI);
    m_azimuth = fmod(m_azimuth + 360, 360);
}

void CUav::calculateSpeedByAxis() 
{
    /* Calculate speed by axis */
    m_velocityX = m_velocity * cos(m_azimuth * glb_PI / 180);
    m_velocityY = m_velocity * sin(m_azimuth * glb_PI / 180);
}

bool CUav::distanceToTarget() 
{
    double distanceToTarget = sqrt(pow(m_destination.m_y - m_currentLocation.m_y, 2) + pow(m_destination.m_x - m_currentLocation.m_x, 2));
    return distanceToTarget <= m_turningRadius ? true : false;
}

void CUav::moveUavInCircle(double deltaTime) 
{
    // Calculate the angular speed and the angle of the movement    
    double angularSpeed = m_velocity / m_turningRadius;
    m_theta += angularSpeed * deltaTime;
    
    // Ensure theta is within valid range [0, 2*pi)
    m_theta = fmod(m_theta, 2 * glb_PI);

    // Calculate new position
    m_currentLocation.m_x = m_destination.m_x + m_turningRadius * cos(m_theta);
    m_currentLocation.m_y = m_destination.m_y + m_turningRadius * sin(m_theta);

    calculateAzimuth();
}