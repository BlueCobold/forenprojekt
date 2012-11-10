#include "ConfigFileLoader.h"

ConfigFileLoader::ConfigFileLoader(void)
{
    m_ResolutionX = 0;
    m_ResolutionY = 0;
    m_BitsPerPixel = 0;
    m_IsFullScreen = false;
}

ConfigFileLoader::~ConfigFileLoader(void)
{
}

int ConfigFileLoader::GetResolutionX()
{
    return m_ResolutionX;
}

int ConfigFileLoader::GetResolutionY()
{
    return m_ResolutionY;
}

int ConfigFileLoader::GetBitsPerPixel()
{
    return m_BitsPerPixel;
}

int ConfigFileLoader::GetFullScreen()
{
    return m_IsFullScreen;
}

bool ConfigFileLoader::LoadConfigFile(std::string FileName)
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
                LoadWindowSettings(ConfigFile);
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

bool ConfigFileLoader::LoadWindowSettings(std::ifstream &File)
{
    std::string Input;
    int ParameterCounter = 4; /// at the moment 4 Parameter for Window Settings
    do
    {
        std::getline(File, Input);
        if(Input.find("ResolutionX = ") != -1)
        {
            if(SetResolutionX(Input))
                ParameterCounter--;
        }
        else if(Input.find("ResolutionY = ") != -1)
        {
            if(SetResolutionY(Input))
                ParameterCounter--;
        }
        else if(Input.find("BitsPerPixel = ") != -1)
        {
            if(SetBitsPerPixel(Input))
                ParameterCounter--;
        }
        else if(Input.find("IsFullScreen = ") != -1)
        {
            if(SetFullScreen(Input))
                ParameterCounter--;
        }
        Input.clear();
    }
    while(!File.eof() &&  ParameterCounter);            
    if(ParameterCounter == 0)
        return true;
    return false;
}

bool ConfigFileLoader::SetResolutionX(std::string &Input)
{
    Input.erase(0, 14 + Input.find("ResolutionX = ")); /// Erase overhead in the string
    std::stringstream toNumber(Input);
    toNumber >> m_ResolutionX; /// Convert string to an integer
    if(m_ResolutionX > 0)
        return true;
    return false;
}

bool ConfigFileLoader::SetResolutionY(std::string &Input)
{
    Input.erase(0, 14 + Input.find("ResolutionY = ")); /// Erase overhead in the string
    std::stringstream toNumber(Input);
    toNumber >> m_ResolutionY; /// Convert string to an integer
    if(m_ResolutionY > 0)
        return true;
    return false;
}

bool ConfigFileLoader::SetBitsPerPixel(std::string &Input)
{
    Input.erase(0, 15 + Input.find("BitsPerPixel = ")); /// Erase overhead in the string
    std::stringstream toNumber(Input);
    toNumber >> m_BitsPerPixel; /// Convert string to an integer
    if(m_BitsPerPixel > 0)
        return true;
    return false;
}

bool ConfigFileLoader::SetFullScreen(std::string &Input)
{
    if(Input.find("false") != -1)
    {
        m_IsFullScreen = false;
        return true;
    }
    if(Input.find("true") != -1)
    {
        m_IsFullScreen = true;
        return true;
    }
    return false;
}