#pragma once

#ifndef CFILEHANDLER_H
#define CFILEHANDLER_H

#include "IWriteDataHandler.h"
#include <fstream>
#include <string>


class CFileHandler : public IWriteDataHandler {
private:
    std::ofstream m_fileStream;

public:
    CFileHandler();
    CFileHandler(CFileHandler&& other);

    virtual bool initHandler(const std::string& fileName) override;
    virtual bool writeHandler(const std::string& data) override;
    virtual bool closeHandler() override;
};

#endif 
