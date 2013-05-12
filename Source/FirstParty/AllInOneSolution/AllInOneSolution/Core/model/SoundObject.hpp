#pragma once

#ifndef SOUND_OBJECT
#define SOUND_OBJECT

#include "SoundManager.hpp"

/// This class is used to be link to the sound manager

class SoundObject
{
public:

    SoundObject();

    void bindSound(const std::string& key, SoundManager* soundManager);
    SoundManager*  getSoundManager();
    std::string getSoundName();

    void fixVolume(float volume);
    void unfixVolume();
    float getVolume(float impactStrength);

private:
    
    std::string m_soundName;
    SoundManager* m_soundManager;

    bool m_volumeFixed;
    float m_volume;
};

#endif // SOUND_OBJECT
