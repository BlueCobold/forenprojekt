#include "AppConfig.hpp"

AppConfig::AppConfig(const std::string& config, const std::string& stash) :
    m_config(config),
    m_stash(stash)
{
}

AppConfig::~AppConfig()
{
}

Config& AppConfig::getConfig()
{
    return m_config;
}

void AppConfig::save()
{
    m_config.save();
    m_stash.save();
}
