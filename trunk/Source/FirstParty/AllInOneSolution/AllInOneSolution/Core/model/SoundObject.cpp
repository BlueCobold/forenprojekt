#include "SoundObject.hpp"

SoundObject::SoundObject() : m_soundManager(nullptr)
{
}

SoundObject::~SoundObject()
{
}

void SoundObject::bindSound(const std::string& name, SoundManager* soundManager)
{
    m_soundManager = soundManager;
    m_soundName = name;
}

const std::string& SoundObject::getSoundName()
{
    return m_soundName;
}

void SoundObject::playSound(const float volume)
{
    if(m_soundName.length() <= 0 || m_soundManager == nullptr)
        return;

    if(m_volumeFixed)
        m_soundManager->play(m_soundName, m_volume);
    else
        m_soundManager->play(m_soundName, volume);
}

void SoundObject::fixVolume(const float volume)
{
    m_volumeFixed = true;
    m_volume = volume;
}

void SoundObject::unfixVolume()
{
    m_volumeFixed = false;
}

bool SoundObject::hasFixedVolume() const
{
    return m_volumeFixed;
}
