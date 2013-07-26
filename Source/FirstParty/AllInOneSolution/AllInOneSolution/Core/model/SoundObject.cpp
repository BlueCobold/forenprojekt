#include "SoundObject.hpp"

#include "../Utility.hpp"

#include <exception>

SoundObject::SoundObject(const std::string& name, SoundManager& soundManager) :
    m_soundManager(soundManager),
    m_soundName(name),
    m_volumeFixed(false),
    m_volume(100)
{
    if(m_soundName.length() <= 0)
        throw std::runtime_error(utility::translateKey("NoSoundName"));
}

SoundObject::SoundObject(const SoundObject& other) :
    m_soundManager(other.m_soundManager),
    m_soundName(other.m_soundName),
    m_volumeFixed(other.m_volumeFixed),
    m_volume(other.m_volume)
{
}

const std::string& SoundObject::getSoundName() const
{
    return m_soundName;
}

SoundManager& SoundObject::getSoundManager() const
{
    return m_soundManager;
}

void SoundObject::play()
{
    play(100.0f);
}

void SoundObject::play(const float volume)
{
    if(m_volumeFixed)
        m_soundManager.play(m_soundName, m_volume);
    else
        m_soundManager.play(m_soundName, volume);
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
