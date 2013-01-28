#include "Config.hpp"

Config::Config(const std::string& fileName) :
    m_fileName(fileName)
{
    readFile();
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

void Config::reload(const std::string& fileName)
{
    m_fileName = fileName;
    
    readFile();
}

void Config::readFile()
{
    // Open file
    std::ifstream configFile(m_fileName, std::ios_base::in);

    if(configFile.is_open())
    {
        // Reset variables
        std::string line = "";
        std::string key = "";
        std::string value = "";
        unsigned int pos = 0;

        while(!configFile.eof())
        {
            std::getline(configFile, line);

            pos = line.find('=');

            // Does not contain a =
            if(pos == std::string::npos)
                continue;

            key = line.substr(0, pos);
            eraseOverhang(key);
            value = line.substr(pos+1);
            eraseOverhang(value);

            if(!key.empty() && !value.empty())
                m_content.insert(std::make_pair<std::string&, std::string&>(key, value));
        } 
    } 
    else
        throw std::runtime_error("File '" + m_fileName + "' not found");
}