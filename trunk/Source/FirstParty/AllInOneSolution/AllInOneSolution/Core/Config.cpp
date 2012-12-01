#include "Config.hpp"

Config::Config(const std::string& fileName) :
    m_fileName(fileName)
{
    readFile();
}

Config::~Config(void)
{
    
}

std::string Config::eraseOverhang(std::string& data)
{
    while(!data.find(" "))             /// Find and erase leading spaces
        data.erase(0, 1);

    unsigned int pos = data.find_last_of(" "); 
    
    while(pos == data.length() - 1)    /// Find and erase spaces after data
    {
        data.erase(pos,pos);
        pos = data.find_last_of(" ");
    }
  
    return data;
}

bool Config::reload(const std::string& fileName)
{
    m_fileName = fileName;

    if(m_configFile)
        m_configFile.close();
    
    return readFile();
}

bool Config::readFile()
{
    m_configFile.open(m_fileName, std::ios_base::in);
    if(m_configFile.is_open())
    {
        std::string line = "";
        std::string key = "";
        std::string value = "";
        unsigned int pos;

        while(!m_configFile.eof())
        {
            std::getline(m_configFile, line);

            pos = line.find('=');

            if(pos == std::string::npos)
                continue;

            key = line.substr(0, pos-1);
            eraseOverhang(key);
            value = line.substr(pos+1);
            eraseOverhang(value);

            if(!key.empty() && !value.empty())
                m_content.insert(std::make_pair<std::string&, std::string&>(key, value));
        }
        m_configFile.close();
        return true;
    }
    m_configFile.close();
    return false;
}