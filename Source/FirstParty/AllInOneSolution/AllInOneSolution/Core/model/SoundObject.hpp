#pragma once

#ifndef SOUND_OBJECT
#define SOUND_OBJECT

#include "SoundManager.hpp"

/// This class is used to link to the sound manager
class SoundObject
{
public:

    SoundObject();
    virtual ~SoundObject();

    void bindSound(const std::string& key, SoundManager* soundManager);

    void fixVolume(const float volume);
    void unfixVolume();
    bool hasFixedVolume() const;
    void playSound(const float volume);

protected:

    SoundManager* getSoundManager();
    const std::string& getSoundName();

private:

    std::string m_soundName;
    SoundManager* m_soundManager;

    bool m_volumeFixed;
    float m_volume;
};

#endif // SOUND_OBJECT
