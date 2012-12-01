#pragma once

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Utility.hpp" // stringTo

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>


/// This class will load configdata from a ini file
class Config
{
public:

    Config(const std::string& fileName);

    template<typename T>
    T get(const std::string& data);
    void reload(const std::string& fileName);

private:

    std::string eraseOverhang(std::string& data);

    void readFile();

private:

    std::string m_fileName;
    std::ifstream m_configFile;
    std::unordered_map<std::string, std::string> m_content;
};

template<typename T>
T Config::get(const std::string& key)
{
    T output = utility::stringTo<T>("0");

    auto it = m_content.find(key);

    if(it != m_content.end())
        output = utility::stringTo<T>(it->second);

    return output;
}

#endif // CONFIG_HPP
