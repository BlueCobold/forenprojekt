#pragma once

#ifndef SOUND_OBJECT
#define SOUND_OBJECT

#include <SFML/Audio/Music.hpp>

/// This class is used to be a container and a manager for a sound for a single entity

class SoundObject
{
public:

    ~SoundObject(){};

    void bindSound(sf::Music* sound);

    void play();

    void pause();

    void stop();


private:
    std::unique_ptr<sf::Music> m_sound;
};

#endif // SOUND_OBJECT
