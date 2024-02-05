#include "CUav.h"
#include <iostream>
#include <sstream>
#include <iomanip> // setprecision function

CUav::CUav() : x(0), y(0), z(0), speed(0), azimuth(0), uavNumber(0), turningRadius(0), destination(0,0),
        gotFirstCommand(false), gotNewCommand(false), speedX(0), speedY(0) {}

void CUav::initialize(const UavParams& params) {
    this->uavNumber = params.uavNumber;
    this->turningRadius = params.radius;
    this->x = params.x0;
    this->y = params.y0;
    this->z = params.z0;
    this->speed = params.speed0; 
}

void CUav::executeCommand(const Command& cmd) {
    /* Make sure we are working with the correct UAV */
    if (cmd.uavNumber == uavNumber) {
        /* Set new destination */
        destination = make_pair(cmd.x, cmd.y);
        if(gotFirstCommand) gotNewCommand = true;
    }
}

void CUav::update(double deltaTime) {
    /* Calculate new azimuth only if there was a new command */
    if (!gotFirstCommand) {
        calculateAzimuth();
        calculateSpeedByAxis();
        gotFirstCommand = true;
    }
    else if (gotNewCommand) {
        calculateAzimuth();
        calculateSpeedByAxis();
        gotNewCommand = false;
    }
    moveUAV(deltaTime);
}

void CUav::moveUAV(double deltaTime) {
    x += speedX * deltaTime;
    y += speedY * deltaTime;
}

void CUav::calculateAzimuth() {
    /* Calculation of delta x and delta y */
    double deltaX = destination.first - x;
    double deltaY = destination.second - y;
    /* Calculation of the azimuth in radians and turn into degrees */
    azimuth = atan2(deltaY, deltaX) * (180.0 / PI);
}

void CUav::calculateSpeedByAxis() {
    /* Calculate speed by asix*/
    speedX = speed * cos(azimuth / 180.0 * PI);
    speedY = speed * sin(azimuth / 180.0 * PI);
}
