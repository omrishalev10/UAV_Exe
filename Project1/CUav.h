#ifndef CUAV_H
#define CUAV_H

#include "IUav.h"
#include <utility>
#include <cmath>

using namespace std;
static const double PI = 2 * acos(0.0);

// UavParams Struct for UAV-specific parameters
struct UavParams {
    int uavNumber;
    double radius;
    double x0;
    double y0;
    double z0;
    double speed0;
    double azimuth;
};

class CUav : public IUav<UavParams>{
private:
    int uavNumber;
    double x, y, z; // Current position
    double speed;   // Current speed
    double azimuth; // Current azimuth
    double turningRadius;
    pair<double, double> destination;
    bool isCircling;

    double calculateAzimuthTowards(double fromX, double fromY, double toX, double toY);
    void adjustAzimuthTowardsDestination();
    void moveUAV(double deltaTime);
    void checkAndInitiateCircling();
    void circleAroundDestination();

public:

    CUav();
    virtual void initialize(const UavParams& params); 
    virtual void update(double deltaTime) override;
    virtual void executeCommand(const Command& cmd) override;

    /* Getters */
    int getUavNumber() { return uavNumber; }
    double getX() { return x; }
    double getY() { return y; }
    double getAzimuth() { return azimuth; }
};

#endif 
