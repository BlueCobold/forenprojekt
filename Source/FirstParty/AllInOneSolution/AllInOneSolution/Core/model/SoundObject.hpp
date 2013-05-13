#pragma once

#ifndef SOUND_OBJECT
#define SOUND_OBJECT

#include "SoundManager.hpp"

/// This class is used to link to the sound manager
class SoundObject
{
public:

    SoundObject(const std::string& key, SoundManager& soundManager);
    virtual ~SoundObject();

    void fixVolume(const float volume);
    void unfixVolume();
    bool hasFixedVolume() const;
    void play(const float volume);

protected:

    SoundManager& getSoundManager() const;
    const std::string& getSoundName() const;

private:

    std::string m_soundName;
    SoundManager& m_soundManager;

    bool m_volumeFixed;
    float m_volume;
};

#endif // SOUND_OBJECT
