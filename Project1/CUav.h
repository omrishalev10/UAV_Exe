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

class CUav : public IUav<UavParams> {
public:

    CUav();
    virtual void initialize(const UavParams& params);
    virtual void executeCommand(const Command& cmd) override;
    virtual void update(double deltaTime) override;


private:
    int uavNumber;
    double x, y, z; // Current position
    double speed;   // Current speed
    double azimuth; // Current azimuth
    double turningRadius;
    pair<double, double> destination;

    bool gotFirstCommand; // flag to sign when got new command to calculate new azimuth
    bool gotNewCommand; // flag to sign when got new command to calculate new azimuth
    bool isCircling;
    /* Axis speed */
    double speedX, speedY;

    void moveUAV(double deltaTime);
    void calculateAzimuth();
    void calculateSpeedByAxis();

public:
    /* Getters */
    int getUavNumber() { return uavNumber; }
    double getX() { return x; }
    double getY() { return y; }
    double getAzimuth() { return azimuth; }
};

#endif 