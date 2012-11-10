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
    int m_ResolutionX;
    int m_ResolutionY;
    int m_BitsPerPixel;
    bool m_IsFullScreen;

    bool LoadWindowSettings(std::ifstream &File);
    bool SetResolutionX(std::string &Input);
    bool SetResolutionY(std::string &Input);
    bool SetBitsPerPixel(std::string &Input);
    bool SetFullScreen(std::string &Input);

public:
    bool LoadConfigFile(std::string FileName);
    int GetResolutionX();
    int GetResolutionY();
    int GetBitsPerPixel();
    int GetFullScreen();
    ConfigFileLoader(void);
    ~ConfigFileLoader(void);
};

#endif /// CONFIGFILELOADER_H