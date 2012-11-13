#pragma once

#ifndef Config_HPP
#define Config_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "Util.hpp"

/// This class will load configdata from a file
class Config
{
private:
    std::string m_fileName;
    std::ifstream m_configFile;

private:
    std::string eraseOverhang(std::string& data);

public:
    template<typename T>
    T get(const std::string& data);
    bool load(std::string& fileName);
    Config(const std::string& fileName);
    ~Config(void);
};

template<typename T>
T Config::get(const std::string& data)
{ 
    std::string input;
    T output = 0;
    if(m_configFile)
    {
        m_configFile.seekg(std::ios_base::beg);
        while(!m_configFile.eof())
        {
            std::getline(m_configFile, input);
            if(input.find(data) != std::string::npos)
            {
                input = eraseOverhang(input);
                output = Util::stringTo<T>(input);
                return output;
            }
            input.clear();
        }
    }
    /// avoid errors if data not found
    output = Util::stringTo<T>("0");
    return output;
}

#endif /// Config_HPP