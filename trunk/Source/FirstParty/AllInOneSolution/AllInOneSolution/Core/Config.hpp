#pragma once

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Utility.hpp" // stringTo

#include <string>
#include <fstream>
#include <sstream>


/// This class will load configdata from a file
class Config
{
public:

    Config(const std::string& fileName);
    ~Config(void);

    template<typename T>
    T get(const std::string& data);
    bool load(const std::string& fileName);

private:

    std::string eraseOverhang(std::string& data);

private:

    std::string m_fileName;
    std::ifstream m_configFile;

};

template<typename T>
T Config::get(const std::string& data)
{ 
    std::string input;
    T output = utility::stringTo<T>("0");

    if(m_configFile)
    {
        m_configFile.seekg(std::ios_base::beg);
        while(!m_configFile.eof())
        {
            std::getline(m_configFile, input);
            if(input.find(data) != std::string::npos)
            {
                input = eraseOverhang(input);
                output = utility::stringTo<T>(input);
                return output;
            }
            input.clear();
        }
    }
    // avoid errors if data not found
    return output;
}

#endif // CONFIG_HPP
