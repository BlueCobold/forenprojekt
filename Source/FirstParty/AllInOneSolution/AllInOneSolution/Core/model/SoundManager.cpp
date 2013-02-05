#include "SoundManager.hpp"

#include <SFML/Audio/SoundBuffer.hpp>

SoundManager::SoundManager(ResourceManager& resourceManager)
    : m_resourceManager(resourceManager)
{
}

void SoundManager::update()
{
    for(auto it = m_sounds.begin(); it != m_sounds.end(); it++)
    {
        it->play();
    }
}

void SoundManager::play(const std::string& key)
{
    m_sounds.push_front(sf::Sound(*m_resourceManager.getSoundBuffer(key)));
}

