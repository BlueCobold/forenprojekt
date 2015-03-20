#include "Config.hpp"
#include "PathHelper.hpp"

Config::Config(const std::string& fileName) :
    FileReader(fileName, true)
{
}

Config::~Config()
{
    save();
}

void Config::save()
{
#ifdef IOS
    std::ofstream configFile(documentPath() + utility::replace(getFileName(), "/", "_"), std::ios_base::out | std::ios_base::trunc);
#else
    std::ofstream configFile(resourcePath() + getFileName(), std::ios_base::out | std::ios_base::trunc);
#endif
    if(configFile.is_open())
    {
        for(auto it = FileReader::beginIterator(); it != FileReader::endIterator(); it++)
            configFile << it->first << " = " << it->second << "\n";
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("MissingFile"), getFileName()));
}
