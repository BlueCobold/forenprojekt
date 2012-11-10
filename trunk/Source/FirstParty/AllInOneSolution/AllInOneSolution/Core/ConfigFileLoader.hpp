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
    int m_resolutionX;
    int m_resolutionY;
    int m_bitsPerPixel;
    bool m_isFullScreen;

    bool loadWindowSettings(std::ifstream &File);
    bool setResolutionX(std::string &Input);
    bool setResolutionY(std::string &Input);
    bool setBitsPerPixel(std::string &Input);
    bool setFullScreen(std::string &Input);

public:
    bool loadConfigFile(std::string FileName);
    int getResolutionX();
    int getResolutionY();
    int getBitsPerPixel();
    int getFullScreen();
    ConfigFileLoader(void);
    ~ConfigFileLoader(void);
};

#endif /// CONFIGFILELOADER_H