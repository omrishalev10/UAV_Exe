#ifndef IUAV_H
#define IUAV_H

#include "SimParser.h"

template <typename T>
class IUav {
public:
    virtual void initialize(const T& params) = 0;
    virtual void update(double deltaTime) = 0;
    virtual void executeCommand(const Command& cmd) = 0;
    virtual ~IUav() {}
};

#endif 