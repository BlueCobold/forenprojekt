#include "ConfigFileLoader.hpp"

ConfigFileLoader::ConfigFileLoader(void)
{
    m_resolutionX = 0;
    m_resolutionY = 0;
    m_bitsPerPixel = 0;
    m_isFullScreen = false;
}

ConfigFileLoader::~ConfigFileLoader(void)
{
}

int ConfigFileLoader::getResolutionX()
{
    return m_resolutionX;
}

int ConfigFileLoader::getResolutionY()
{
    return m_resolutionY;
}

int ConfigFileLoader::getBitsPerPixel()
{
    return m_bitsPerPixel;
}

int ConfigFileLoader::getFullScreen()
{
    return m_isFullScreen;
}

bool ConfigFileLoader::loadConfigFile(std::string FileName)
{ 
    std::ifstream ConfigFile;
    std::string Input;
    int GroupCounter = 1; /// at the moment 1 group of parameter in the ConfigFile
    ConfigFile.open(FileName, std::ios_base::in);
    if(ConfigFile)
    {
        do
        {
            std::getline(ConfigFile, Input);
            if(Input == "[Window]")
            {
                loadWindowSettings(ConfigFile);
                GroupCounter--;
            }
            Input.clear();
        }
        while(!ConfigFile.eof() && GroupCounter);
    }
    if(GroupCounter == 0)
        return true;
    return false;
}

bool ConfigFileLoader::loadWindowSettings(std::ifstream &File)
{
    std::string Input;
    int ParameterCounter = 4; /// at the moment 4 Parameter for Window Settings
    do
    {
        std::getline(File, Input);
        if(Input.find("ResolutionX = ") != -1)
        {
            if(setResolutionX(Input))
                ParameterCounter--;
        }
        else if(Input.find("ResolutionY = ") != -1)
        {
            if(setResolutionY(Input))
                ParameterCounter--;
        }
        else if(Input.find("BitsPerPixel = ") != -1)
        {
            if(setBitsPerPixel(Input))
                ParameterCounter--;
        }
        else if(Input.find("IsFullScreen = ") != -1)
        {
            if(setFullScreen(Input))
                ParameterCounter--;
        }
        Input.clear();
    }
    while(!File.eof() &&  ParameterCounter);            
    if(ParameterCounter == 0)
        return true;
    return false;
}

bool ConfigFileLoader::setResolutionX(std::string &Input)
{
    Input.erase(0, 14 + Input.find("ResolutionX = ")); /// Erase overhead in the string
    std::stringstream toNumber(Input);
    toNumber >> m_resolutionX; /// Convert string to an integer
    if(m_resolutionX > 0)
        return true;
    return false;
}

bool ConfigFileLoader::setResolutionY(std::string &Input)
{
    Input.erase(0, 14 + Input.find("ResolutionY = ")); /// Erase overhead in the string
    std::stringstream toNumber(Input);
    toNumber >> m_resolutionY; /// Convert string to an integer
    if(m_resolutionY > 0)
        return true;
    return false;
}

bool ConfigFileLoader::setBitsPerPixel(std::string &Input)
{
    Input.erase(0, 15 + Input.find("BitsPerPixel = ")); /// Erase overhead in the string
    std::stringstream toNumber(Input);
    toNumber >> m_bitsPerPixel; /// Convert string to an integer
    if(m_bitsPerPixel > 0)
        return true;
    return false;
}

bool ConfigFileLoader::setFullScreen(std::string &Input)
{
    if(Input.find("false") != -1)
    {
        m_isFullScreen = false;
        return true;
    }
    if(Input.find("true") != -1)
    {
        m_isFullScreen = true;
        return true;
    }
    return false;
}