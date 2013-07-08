#pragma once

#ifndef SOUND_BUFFER_MANAGER
#define SOUND_BUFFER_MANAGER

#include <string>
#include <SFML/Audio/SoundBuffer.hpp>

class SoundBufferManager
{
public:

    virtual sf::SoundBuffer* getSoundBuffer(const std::string& key) = 0;
};

#endif // SOUND_BUFFER_MANAGER
