#include "MusicManager.hpp"
#include <SFML/Audio/Music.hpp>

MusicManager::MusicManager(std::vector<sf::Music*>& music, MusicManager::PlayMode mode) :
    m_music(music),
    m_randomGenerator(0.f, static_cast<float>(music.size())),
    m_mode(mode),
    m_isPlaying(false),
    m_currentIndex(0)
{
    if(m_mode == MusicManager::Shuffle)
        m_currentIndex = getRandomNumber();
}

void MusicManager::update()
{
    if(!m_isPlaying || m_music[m_currentIndex]->getStatus() == sf::Music::Playing)
        return;

    if(m_mode == MusicManager::Normal)
    {
        if(m_currentIndex + 1 < m_music.size())
            ++m_currentIndex;
        else
            m_currentIndex = 0;
    }
    else
        m_currentIndex = getRandomNumber();

    m_music[m_currentIndex]->play();
}

void MusicManager::play()
{
    m_isPlaying = true;
    m_music[m_currentIndex]->play();
}

void MusicManager::stop()
{
    m_isPlaying = false;
    m_music[m_currentIndex]->stop();
}

void MusicManager::pause()
{
    m_isPlaying = false;
    m_music[m_currentIndex]->pause();
}

void MusicManager::next()
{
    m_isPlaying = true;
    m_music[m_currentIndex]->stop();
}

void MusicManager::prev()
{
    m_isPlaying = true;
    m_music[m_currentIndex]->stop();
    
    if(m_mode == MusicManager::Shuffle)
        return;

    if(m_currentIndex - 1 >= 0)
        m_music[--m_currentIndex]->play();
}

unsigned int MusicManager::getRandomNumber()
{
    return static_cast<unsigned int>(m_randomGenerator.getValue());
}