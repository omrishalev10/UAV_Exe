#include "CFileHandler.h"
#include <iostream>

CFileHandler::CFileHandler() {}

CFileHandler::~CFileHandler() {
    closeHandler();
}

/* Open a file with the given name */
bool CFileHandler::initHandler(const string& fileName) {
    fileStream.open(fileName, ios::out);
    return fileStream.is_open();
}

/* Write to the file the given data */
bool CFileHandler::writeHandler(const string& data) {
    if (!fileStream.is_open()) {
        cerr << "Error: File is not open for writing." << endl;
        return false;
    }
    fileStream << data << endl;
    if (fileStream.fail()) {
        cerr << "Error: Write operation failed." << endl;
        return false;
    }
    return true;
}

/* If file is open - close it*/
bool CFileHandler::closeHandler() {
    if (fileStream.is_open()) {
        fileStream.close();
    }
    return !fileStream.is_open();
}
