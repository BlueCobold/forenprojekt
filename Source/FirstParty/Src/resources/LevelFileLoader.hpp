#pragma once

#ifndef LEVEL_FILE_LOADER_HPP
#define LEVEL_FILE_LOADER_HPP


#include <memory>
#include <string>
#include <vector>

#include <tinyxml2.h>

class LevelFileLoader
{
public:

    LevelFileLoader()
    { }

    static std::vector<std::string> parseGrid(const tinyxml2::XMLElement& xml);
};

#endif // LEVEL_FILE_LOADER_HPP
