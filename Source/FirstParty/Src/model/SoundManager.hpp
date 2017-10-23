#pragma once

#ifndef SOUND_MANAGER
#define SOUND_MANAGER

#include "../resources/SoundBufferManager.hpp"

#include <SFML/Audio/Sound.hpp>

#include <queue>
#include <string>
#include <map>
#include <memory>

class AppConfig;

/// This class will be used to manage all the sounds
/// of the game
class SoundManager 
{
public:
    SoundManager(SoundBufferManager& resourceManager, AppConfig& config);
    ~SoundManager();

    void update();
    void play(const std::string& key, const float volume);
    void clear();

private:
    SoundBufferManager& m_resourceManager;
    AppConfig& m_config;
#ifndef NO_SOUND
    std::queue<std::pair<std::string, std::unique_ptr<sf::Sound>>> m_sounds;
#endif
    std::map<std::string, std::string> m_soundKeys;
};

#endif // SOUND_MANAGER
