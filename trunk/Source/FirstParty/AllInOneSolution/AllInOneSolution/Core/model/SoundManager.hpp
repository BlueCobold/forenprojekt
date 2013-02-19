#ifndef SOUND_MANAGER
#define SOUND_MANAGER

#pragma once

#include "../resources/ResourceManager.hpp"

#include <SFML/Audio/Sound.hpp>

#include <queue>
#include <string>
#include <map>

/// This class will be used to manage all the sounds
/// of the game
class SoundManager 
{
public:
	SoundManager(ResourceManager& resourceManager);

    void update();
	void play(const std::string& key, const float vel);

private:
	ResourceManager& m_resourceManager;
	std::queue< std::pair<std::string, sf::Sound> > m_sounds;
    std::map<std::string, std::string> m_soundKeys;

};

#endif // SOUND_MANAGER