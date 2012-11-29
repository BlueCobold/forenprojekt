#include "Config.hpp"

Config::Config(const std::string& fileName) :
    m_fileName(fileName)
{
    m_configFile.open(m_fileName, std::ios_base::in);
}

Config::~Config(void)
{
    m_configFile.close();
}

std::string Config::eraseOverhang(std::string& data)
{
    data.erase(0, data.find("=") + 2); /// Erase key in the string
    return data;
}

bool Config::load(const std::string& fileName)
{
    m_fileName = fileName;
    if(m_configFile)
        m_configFile.close();
    m_configFile.open(m_fileName, std::ios_base::in);
    if(m_configFile)
        return true;
    else
        return false;
}
