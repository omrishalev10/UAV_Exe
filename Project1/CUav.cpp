#include "CUav.h"
#include <iostream>
#include <sstream>
#include <iomanip> // setprecision function

CUav::CUav() : m_currentLocation({ 0.0, 0.0, 0.0 }), m_velocity(0), m_azimuth(0), m_uavNumber(0), m_turningRadius(0),
        m_destination({ 0.0, 0.0 }), m_isNewCommand(false), m_isFirstCommand(false), isCircling(false), m_velocityX(0),
        m_velocityY(0), m_theta(0), m_state(MovementState::MovingForward) {}

void CUav::initialize(const UavParams& params)
{
    this->m_uavNumber = params.m_uavNumber;
    this->m_turningRadius = params.m_radius;
    this->m_currentLocation.m_x = params.m_x0;
    this->m_currentLocation.m_y = params.m_y0;
    this->m_currentLocation.m_z = params.m_z0;
    this->m_velocity = params.m_velocity0;
    this->m_azimuth = params.m_azimuth;

    calculateSpeedByAxis();
}

void CUav::executeCommand(const Command& cmd) 
{
    if (cmd.m_uavNumber == m_uavNumber)
    {
        m_destination = { cmd.m_x, cmd.m_y };
        m_isNewCommand = true;
        m_state = MovementState::Turn;
    }
}

void CUav::update(double deltaTime) 
{
    switch (m_state) 
    {
    case MovementState::MovingForward:
        if (m_isNewCommand) 
        {
            calculateAzimuth();
            m_isNewCommand = false;
            m_isFirstCommand = true; 
            calculateSpeedByAxis(); 
        }
        moveUAV(deltaTime);
        if (m_isFirstCommand && distanceToTarget()) 
        {
            m_state = MovementState::Circling; 
        }
        break;

    case MovementState::Circling:
        moveUavInCircle(deltaTime); 
        break;

    case MovementState::Turn:
        moveUavNewDirection(deltaTime);
        break;
    default:
        break;
    }
}

void CUav::moveUavNewDirection(double deltaTime) {
    // Determine the target azimuth towards the destination
    double targetAzimuthRadians = atan2(m_destination.m_y - m_currentLocation.m_y, m_destination.m_x - m_currentLocation.m_x);
    double targetAzimuthDegrees = fmod(targetAzimuthRadians * (180.0 / glb_PI) + 360.0, 360.0);

    // Current azimuth in radians for calculation
    double currentAzimuthRadians = m_azimuth * (glb_PI / 180.0);

    // Calculate the angular distance to turn
    double angularDistance = targetAzimuthRadians - m_azimuth * (glb_PI / 180.0);

    // Ensure the angular distance is within the range [-pi, pi] for shortest turn
    if (angularDistance > glb_PI) {
        angularDistance -= 2 * glb_PI;
    }
    else if (angularDistance < -glb_PI) {
        angularDistance += 2 * glb_PI;
    }

    // Calculate the angular speed based on the turning radius and velocity
    double angularSpeed = m_velocity / m_turningRadius; // radians per second

    // Calculate how much the UAV can turn this update, considering deltaTime
    double turnThisUpdate = angularSpeed * deltaTime;

    // Do not turn more than needed to align with the target
    if (fabs(turnThisUpdate) > fabs(angularDistance)) {
        turnThisUpdate = angularDistance;
    }

    // Update UAV's azimuth attribute, converting back to degrees and ensuring it's within [0, 360)
    m_azimuth = fmod((m_azimuth * (glb_PI / 180.0) + turnThisUpdate) * (180.0 / glb_PI) + 360.0, 360.0);

    // After adjusting azimuth, update velocity components based on new azimuth
    calculateSpeedByAxis();

    // Move the UAV forward after adjusting its direction
    m_currentLocation.m_x += m_velocityX * deltaTime;
    m_currentLocation.m_y += m_velocityY * deltaTime;

    // Check if the UAV has aligned with the target azimuth, if so, switch to moving forward
    if (fabs(targetAzimuthDegrees - m_azimuth) < 1.0) { // A small threshold to consider alignment
        m_state = MovementState::MovingForward;
    }
}


void CUav::moveUAV(double deltaTime) 
{
    calculateSpeedByAxis();
    m_currentLocation.m_x += m_velocityX * deltaTime;
    m_currentLocation.m_y += m_velocityY * deltaTime;
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
    return distanceToTarget <= m_turningRadius;
}

void CUav::moveUavInCircle(double deltaTime)
{
    // Calculate the angular speed and the angle of the movement    
    double angularSpeed = m_velocity / m_turningRadius;
    m_theta -= angularSpeed * deltaTime;

    // Ensure theta is within valid range [0, 2*pi)
    m_theta = fmod(m_theta, 2 * glb_PI);

    // Calculate new position
    m_currentLocation.m_x = m_destination.m_x + m_turningRadius * cos(m_theta);
    m_currentLocation.m_y = m_destination.m_y + m_turningRadius * sin(m_theta);

    calculateAzimuth();
}