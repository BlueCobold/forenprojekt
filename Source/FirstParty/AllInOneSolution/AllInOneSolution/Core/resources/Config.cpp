#include "Config.hpp"

Config::Config(const std::string& fileName) :
FileReader(fileName)
{
}

void Config::save()
{
    std::ofstream configFile(getFileName(), std::ios_base::out | std::ios_base::trunc);
    if(configFile.is_open())
    {
        for(auto it = FileReader::beginIterator(); it != FileReader::endIterator(); it++)
            configFile << it->first << " = " << it->second << "\n";
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("MissingFile"), getFileName()));
}