#include "ConfigFileLoader.hpp"

ConfigFileLoader::ConfigFileLoader(std::string File)
    : m_FileName(File)
{
}

ConfigFileLoader::~ConfigFileLoader(void)
{
}

std::string ConfigFileLoader:: getString(std::string Data)
{ 
    std::ifstream ConfigFile;
    std::string Input;
    ConfigFile.open(m_FileName, std::ios_base::in);
    if(ConfigFile)
    {
        do
        {
            std::getline(ConfigFile, Input);
            if(Input.find(Data) != -1)
            {
                Input = eraseOverhang(Input);
                return Input;
            }
            Input.clear();
        }
        while(!ConfigFile.eof());
    }
    return "Fehler";
}

int ConfigFileLoader::getInt(std::string Data)
{ 
    std::ifstream ConfigFile;
    std::string Input;
    ConfigFile.open(m_FileName, std::ios_base::in);
    if(ConfigFile)
    {
        do
        {
            std::getline(ConfigFile, Input);
            if(Input.find(Data) != -1)
            {
                Input = eraseOverhang(Input);
                return StringToInt(Input);
            }
            Input.clear();
        }
        while(!ConfigFile.eof());
    }
    return -1;
}
bool ConfigFileLoader::getBool(std::string Data)
{ 
    std::ifstream ConfigFile;
    std::string Input;
    ConfigFile.open(m_FileName, std::ios_base::in);
    if(ConfigFile)
    {
        do
        {
            std::getline(ConfigFile, Input);
            if(Input.find(Data) != -1)
            {
                return StringToBool(Input);
            }
            Input.clear();
        }
        while(!ConfigFile.eof());
    }
    return false;
}
int ConfigFileLoader::StringToInt(std::string Data)
{
    int Number;
    std::stringstream toNumber(Data);
    toNumber >> Number; /// Convert string to an integer
    return Number;
}

std::string ConfigFileLoader::eraseOverhang(std::string Data)
{
    Data.erase(0, Data.find("=") + 1); /// Erase overhang in the string
    do
    {
        Data.replace(Data.find(" "), Data.find(" ") + 1, ""); /// Erase Spaces
    }
    while(Data.find(" ") != -1);
    return Data;
}

bool ConfigFileLoader::StringToBool(std::string Data)
{
    if(Data.find("true") != -1)
        return true;
    else
        return false;
}