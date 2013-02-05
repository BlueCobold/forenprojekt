#ifndef SOUND_MANAGER
#define SOUND_MANAGER

#pragma once

#include "../resources/ResourceManager.hpp"

#include <SFML/Audio/Sound.hpp>

#include <deque>
#include <string>

/// This class will be used to manage all the sounds
/// of the game
class SoundManager 
{
public:
	SoundManager(ResourceManager& resourceManager);

    void update();
	void play(const std::string& key);

private:
	ResourceManager& m_resourceManager;
	std::deque<sf::Sound> m_sounds;

};

#endif // SOUND_MANAGER