#pragma once

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../Utility.hpp" // stringTo

#include "FileReader.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>


/// This class will load configdata from a ini file
class Config : public FileReader
{
public:

    Config(const std::string& fileName);

    ~Config();

    template<typename T>
    T get(const std::string& data);

    template<typename T>
    void set(const std::string& key, const T& value);

    void save();
};

template<typename T>
T Config::get(const std::string& key)
{
    return utility::stringTo<T>(FileReader::get(key));
}

template<typename T>
void Config::set(const std::string& key, const T& value)
{
    FileReader::set(key, utility::toString(value));
}

#endif // CONFIG_HPP
