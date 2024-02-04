#include "CUav.h"
#include <iostream>


CUav::CUav() : uavNumber(0), turningRadius(0), x(0), y(0), z(0), speed(0), azimuth(0) {}

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
    if (cmd.uavNumber == this->uavNumber) {

        /* Set the new destination  and if the plane was circiling so update its not anymore */
        destination = make_pair(cmd.x, cmd.y);
        //isCircling = false;

        /* Update the new azimuth */
       // this->azimuth = calculateAzimuthTowards(x,y, destination.first,destination.second);
    }
}
    
void CUav::update(double deltaTime) {
    //adjustAzimuthTowardsDestination();
    moveUAV(deltaTime);
    //checkAndInitiateCircling();
}

void CUav::moveUAV(double deltaTime) {
    double speedX = speed * cos(azimuth / 180.0 * PI);
    double speedY = speed * sin(azimuth / 180.0 * PI);
    x += speedX * deltaTime;
    y += speedY * deltaTime;
}






void CUav::adjustAzimuthTowardsDestination() {
    double deltaX = destination.first - x;
    double deltaY = destination.second - y;
    double targetAzimuth = atan(deltaY / deltaX) * (180.0 / PI);
    if (targetAzimuth < 0) 
        targetAzimuth += 360.0;

    // Normalize azimuth to [0, 360)
    azimuth = fmod(azimuth + 360, 360);
}

void CUav::checkAndInitiateCircling() {
    double distanceToDest = sqrt(pow(destination.first - x, 2) + pow(destination.second - y, 2));
    if (distanceToDest <= turningRadius && !isCircling) {
        isCircling = true; // Start circling
    }

    if (isCircling) {
        // Implement circling around the destination
        circleAroundDestination();
    }
}

void CUav::circleAroundDestination() {
    // Assuming we adjust the azimuth to circle around with a fixed rate, could be refined
    azimuth += (360.0 / (2 * PI * turningRadius / speed)); // Adjusting for a complete circle depending on speed and turningRadius
    azimuth = fmod(azimuth, 360); // Ensure azimuth remains within valid range
}

double CUav::calculateAzimuthTowards(double fromX, double fromY, double toX, double toY) {
    
    /* Calculation of delta x and delta y */
    double deltaX = toX - fromX;
    double deltaY = toY - fromY;
    /* Calculation of the azimuth in radians and turn into degrees */
    double azimuthDegrees = atan(deltaY / deltaX) / (180.0 * PI);
  //  double azimuthDegrees = azimuthRadians * (180.0 / PI);

    // Normalize azimuth to [0, 360) degrees
    if (azimuthDegrees < 0) azimuthDegrees += 360.0;
    else if (azimuthDegrees >= 360) azimuthDegrees -= 360.0;

    azimuthDegrees = fmod(azimuthDegrees, 360);

    return azimuthDegrees;
}