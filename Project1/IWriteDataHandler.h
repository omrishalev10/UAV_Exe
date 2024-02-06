#ifndef IWRITEDATAHANDLER_H
#define IWRITEDATAHANDLER_H

#include <string>

class IWriteDataHandler {
public:
    virtual bool initHandler(const std::string& Name) = 0; // Open new 
    virtual bool writeHandler(const std::string& data) = 0; // This function can contains any type of file
    virtual bool closeHandler() = 0;
};
#endif 
