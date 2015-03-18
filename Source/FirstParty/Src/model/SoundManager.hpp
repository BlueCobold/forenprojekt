#ifndef SOUND_MANAGER
#define SOUND_MANAGER

#pragma once

#include "../resources/SoundBufferManager.hpp"

#include <SFML/Audio/Sound.hpp>

#include <queue>
#include <string>
#include <map>
#include <memory>

/// This class will be used to manage all the sounds
/// of the game
class SoundManager 
{
public:
    SoundManager(SoundBufferManager& resourceManager);

    void update();
    void play(const std::string& key, const float volume);

private:
    SoundBufferManager& m_resourceManager;
#ifndef NO_SOUND
    std::queue<std::pair<std::string, std::unique_ptr<sf::Sound>>> m_sounds;
#endif
    std::map<std::string, std::string> m_soundKeys;
};

#endif // SOUND_MANAGER