#include "SoundObject.hpp"

void SoundObject::bindSound(const std::string& key, SoundManager* soundManager)
{
    m_soundManager = soundManager;
    m_key = key;
}

std::string SoundObject::getKey()
{
    return m_key;
}

SoundManager* SoundObject::getSoundManager()
{
    return m_soundManager;
}