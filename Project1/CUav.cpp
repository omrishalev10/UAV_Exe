#include "CUav.h"
#include <iostream>
#include <sstream>
#include <iomanip> // setprecision function

CUav::CUav() : x(0), y(0), z(0), speed(0), azimuth(0), uavNumber(0), turningRadius(0), destination(0, 0),
gotNewCommand(false), isFirstCommand(false), speedX(0), speedY(0) {}

void CUav::initialize(const UavParams& params) {
    this->uavNumber = params.uavNumber;
    this->turningRadius = params.radius;
    this->x = params.x0;
    this->y = params.y0;
    this->z = params.z0;
    this->speed = params.speed0;
    this->azimuth = params.azimuth;
}

void CUav::executeCommand(const Command& cmd) 
{
    /* Make sure we are working with the correct UAV */
    if (cmd.uavNumber == uavNumber) 
    {
        /* Set new destination */
        destination = make_pair(cmd.x, cmd.y);
        if (!gotNewCommand) 
        { 
            gotNewCommand = true;
        }
    }
}

void CUav::update(double deltaTime) 
{
    /* Calculate new azimuth only if there was a new command */
    if (gotNewCommand) 
    {
        calculateAzimuth();
        gotNewCommand = false;
        isFirstCommand = true;
    }
    if (isFirstCommand) 
    {
        distanceToTarget();
    }
    moveUAV(deltaTime);
}

void CUav::moveUAV(double deltaTime) 
{ 
    if (isFirstCommand)
    {
        moveUavInCircle(deltaTime);
    }
    else 
    {
        calculateSpeedByAxis();
        x += speedX * deltaTime;
        y += speedY * deltaTime;
    }
}

void CUav::calculateAzimuth() {
    /* Calculation of delta x and delta y */
    double deltaX = destination.first - x;
    double deltaY = destination.second - y;
    /* Calculation of the azimuth in radians and turn into degrees */
    azimuth = atan2(deltaY, deltaX) * (180.0 / PI);
    azimuth = fmod(azimuth + 360, 360);
}

void CUav::calculateSpeedByAxis() {
    /* Calculate speed by axis */
    speedX = speed * cos(azimuth * PI / 180);
    speedY = speed * sin(azimuth * PI / 180);
}

bool CUav::distanceToTarget() 
{
    double distanceToTarget = sqrt(pow(destination.second - y, 2) + pow(destination.first - x, 2));
    return distanceToTarget <= turningRadius;
}

void CUav::moveUavInCircle(double deltaTime) 
{
    double angularSpeed = speed / turningRadius;
    theta += angularSpeed * deltaTime;
    
    // Ensure theta is within valid range [0, 2*pi)
    theta = fmod(theta, 2 * PI);

    // Calculate new position
    x = destination.first + turningRadius * cos(theta);
    y = destination.second + turningRadius * sin(theta);

    calculateAzimuth();
}