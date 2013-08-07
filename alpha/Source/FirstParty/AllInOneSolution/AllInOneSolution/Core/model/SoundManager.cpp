#include "SoundManager.hpp"

#include <vector>

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Listener.hpp>

SoundManager::SoundManager(SoundBufferManager& resourceManager)
    : m_resourceManager(resourceManager)
{
}

void SoundManager::update()
{
    if(!m_sounds.empty())
    {
        if(m_sounds.front().second->getStatus() == sf::Sound::Stopped)
        {
            m_soundKeys.erase(m_sounds.front().first);
            m_sounds.pop();
        }
    }
}

void SoundManager::play(const std::string& key, const float volume)
{
    if(m_soundKeys.count(key) <= 0)
    {
        auto sound = std::unique_ptr<sf::Sound>(new sf::Sound(*m_resourceManager.getSoundBuffer(key)));
        if(volume <= sf::Listener::getGlobalVolume())
            sound->setVolume(volume);
        sound->play();
        m_soundKeys.insert(std::pair<std::string, std::string>(key,key));
        m_sounds.push(std::make_pair(key, std::move(sound)));
    }
}
