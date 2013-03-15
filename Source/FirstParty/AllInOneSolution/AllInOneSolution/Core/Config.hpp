#pragma once

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Utility.hpp" // stringTo

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

    template<typename T>
    T get(const std::string& data);
};

template<typename T>
T Config::get(const std::string& key)
{
    return utility::stringTo<T>(FileReader::get(key));
}

#endif // CONFIG_HPP
