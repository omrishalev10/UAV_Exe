#include "CUav.h"
#include <iostream>

CUav::CUav() : uavNumber(0), turningRadius(0), x(0), y(0), z(0), speed(0), azimuth(0), isCircling(false), GotCommand(false) {}

void CUav::initialize(const UavParams& params) {
    this->uavNumber = params.uavNumber;
    this->turningRadius = params.radius;
    this->x = params.x0;
    this->y = params.y0;
    this->z = params.z0;
    this->speed = params.speed0;
    this->azimuth = params.azimuth;
}

void CUav::executeCommand(const Command& cmd) {
    /* Make sure we are working with the correct UAV */
    if (cmd.uavNumber == uavNumber) {
        /* Set new destination */
        destination = make_pair(cmd.x, cmd.y);
        GotCommand = true;
    }
}

void CUav::update(double deltaTime) {
    moveUAV(deltaTime);
}

void CUav::moveUAV(double deltaTime) {
    /* Calculate new azimuth only if there was a new command */
    if (GotCommand) calculateAzimuth();
    double speedX = speed * cos(azimuth / 180.0 * PI);
    double speedY = speed * sin(azimuth / 180.0 * PI);
    x += speedX * deltaTime;
    y += speedY * deltaTime;
}

void CUav::calculateAzimuth() {
    /* Calculation of delta x and delta y */
    double deltaX = destination.first - x;
    double deltaY = destination.second - y;
    /* Calculation of the azimuth in radians and turn into degrees */
   // azimuth = atan((deltaY/deltaX) / 180.0 * PI);
    azimuth = atan2(deltaY, deltaX) * (180.0 / PI);

}