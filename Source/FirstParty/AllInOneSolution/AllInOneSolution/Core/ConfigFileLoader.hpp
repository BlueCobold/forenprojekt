#pragma once

#ifndef CONFIGFILELOADER_H
#define CONFIGFILELOADER_H

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

/// This class will load configdata from a file
class ConfigFileLoader
{
private:
    std::string m_FileName;

    int StringToInt(std::string Data);
    bool StringToBool(std::string Data);
    std::string eraseOverhang(std::string Data);
public:
    int getInt(std::string Data);
    bool getBool(std::string Data);
    std::string getString(std::string Data);
    ConfigFileLoader(std::string File);
    ~ConfigFileLoader(void);
};

#endif /// CONFIGFILELOADER_H