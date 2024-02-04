#ifndef IWRITEDATAHANDLER_H
#define IWRITEDATAHANDLER_H

#include <string>
using namespace std;

template <typename T>
class IWriteDataHandler {
public:
    virtual bool initHandler(const string& Name) = 0; // Open new ...
    virtual bool writeHandler(const T& data) = 0; // This function can contains any type of file
    virtual bool closeHandler() = 0;
    virtual ~IWriteDataHandler() {}
};

#endif 
