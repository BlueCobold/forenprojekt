#pragma once

#ifndef SOUND_OBJECT
#define SOUND_OBJECT

#include "SoundManager.hpp"

/// This class is used to be link to the sound manager

class SoundObject
{
public:

    void bindSound(const std::string& key, SoundManager* soundManager);
    SoundManager*  getSoundManager();
    std::string getSoundName();

private:
    
    std::string m_soundName;
    SoundManager* m_soundManager;
};

#endif // SOUND_OBJECT
