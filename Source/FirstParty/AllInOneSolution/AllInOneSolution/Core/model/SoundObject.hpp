#pragma once

#ifndef SOUND_OBJECT
#define SOUND_OBJECT

#include <SFML/Audio/Sound.hpp>

/// This class is used to be a container and a manager for a sound for a single entity

class SoundObject
{
public:

    SoundObject();

    virtual ~SoundObject();

    void bindSound(sf::Sound* sound);

    void play();

    void pause();

    void stop();


private:
    sf::Sound* m_sound;
};

#endif // SOUND_OBJECT
