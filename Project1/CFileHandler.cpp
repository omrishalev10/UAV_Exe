#include "CFileHandler.h"
#include <iostream>

CFileHandler::CFileHandler() {}

//CFileHandler::CFileHandler(CFileHandler&& other) : m_fileStream(std::move(other.m_fileStream)) {}

/* Open a file with the given name */
bool CFileHandler::initHandler(const std::string& fileName) {
    m_fileStream.open(fileName, std::ios::out);
    return m_fileStream.is_open();
}

/* Write to the file the given data */
bool CFileHandler::writeHandler(const std::string& data) {
    if (!m_fileStream.is_open()) {
        std::cerr << "Error: File is not open for writing." << std::endl;
        return false;
    }
    m_fileStream << data << std::endl;
    if (m_fileStream.fail()) {
        std::cerr << "Error: Write operation failed." << std::endl;
        return false;
    }
    return true;
}

/* If file is open - close it*/
bool CFileHandler::closeHandler() {
    if (m_fileStream.is_open()) {
        m_fileStream.close();
    }
    return !m_fileStream.is_open();
}
