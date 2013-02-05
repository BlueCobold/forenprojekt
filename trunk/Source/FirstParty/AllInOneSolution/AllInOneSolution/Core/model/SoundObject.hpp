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
    std::string getKey();

private:
    
    std::string m_key;
    SoundManager* m_soundManager;
};

#endif // SOUND_OBJECT
