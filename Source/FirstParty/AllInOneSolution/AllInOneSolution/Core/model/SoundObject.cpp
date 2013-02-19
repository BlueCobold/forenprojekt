#include "SoundObject.hpp"

SoundObject::SoundObject() : m_soundManager(nullptr)
{
}

void SoundObject::bindSound(const std::string& name, SoundManager* soundManager)
{
    m_soundManager = soundManager;
    m_soundName = name;
}

std::string SoundObject::getSoundName()
{
    return m_soundName;
}

SoundManager* SoundObject::getSoundManager()
{
    return m_soundManager;
}