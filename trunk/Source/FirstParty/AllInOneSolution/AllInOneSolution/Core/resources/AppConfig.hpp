#pragma once

#ifndef APPCONFIG_HPP
#define APPCONFIG_HPP

#include "Config.hpp"

class AppConfig
{
private:
    Config m_config;
    Config m_stash;
public:
    AppConfig(const std::string& config, const std::string& stash);
    ~AppConfig();

    template<typename T>
    T get(const std::string& data);

    template<typename T>
    void set(const std::string& key, const T& value);

};

template<typename T>
T AppConfig::get(const std::string& key)
{
    if(m_config.existKey(key))
        return m_config.get<T>(key);
    else
        return m_stash.get<T>(key);
}

template<typename T>
void AppConfig::set(const std::string& key, const T& value)
{
    if(m_config.existKey(key))
        m_config.set<T>(key, value);
    if(m_stash.existKey(key))
        m_stash.set<T>(key, value);
}

#endif
