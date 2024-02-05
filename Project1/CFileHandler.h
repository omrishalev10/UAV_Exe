#pragma once

#ifndef CFILEHANDLER_H
#define CFILEHANDLER_H

#include "IWriteDataHandler.h"
#include <fstream>
#include <string>

using namespace std;

class CFileHandler : public IWriteDataHandler<string> {
private:
    ofstream fileStream;

public:
    CFileHandler();
    CFileHandler(CFileHandler&& other);
    virtual ~CFileHandler();

    virtual bool initHandler(const string& fileName) override;
    virtual bool writeHandler(const string& data) override;
    virtual bool closeHandler() override;
};

#endif 
