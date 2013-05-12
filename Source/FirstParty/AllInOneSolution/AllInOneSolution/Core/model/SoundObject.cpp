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

void SoundObject::fixVolume(float volume)
{
    m_volumeFixed = true;
    m_volume = volume;
}

void SoundObject::unfixVolume()
{
    m_volumeFixed = false;
}

float SoundObject::getVolume(float impactStrength)
{
    if(m_volumeFixed)
        return m_volume;
    return std::min(100.0f, std::max(0.0f, impactStrength));
}