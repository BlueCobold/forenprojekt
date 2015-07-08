#include "MusicPlayer.hpp"
#include <SFML/Audio/Music.hpp>

MusicPlayer::MusicPlayer(std::vector<sf::Music*>& music, MusicPlayer::PlayMode mode) :
    m_music(music),
    m_randomGenerator(0.f, static_cast<float>(music.size())),
    m_mode(mode),
    m_isPlaying(false),
    m_currentIndex(0)
{
    if(m_mode == MusicPlayer::Shuffle)
        m_currentIndex = getRandomNumber();
}

void MusicPlayer::update()
{
    if(!m_isPlaying || 
       m_music[m_currentIndex]->getStatus() == sf::Music::Playing ||
       m_music.empty())
        return;

    if(m_mode == MusicPlayer::Normal)
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

void MusicPlayer::play()
{
    if(!m_music.empty())
    {
        m_isPlaying = true;
        m_music[m_currentIndex]->play();
    }
}

void MusicPlayer::stop()
{
    if(!m_music.empty())
    {
        m_isPlaying = false;
        m_music[m_currentIndex]->stop();
    }
}

void MusicPlayer::pause()
{
    if(!m_music.empty())
    {
        m_isPlaying = false;
        m_music[m_currentIndex]->pause();
    }
}

void MusicPlayer::next()
{
    if(!m_music.empty())
    {
        m_isPlaying = true;
        m_music[m_currentIndex]->stop();
    }
}

void MusicPlayer::previous()
{
    if(!m_music.empty())
    {
        m_isPlaying = true;
        m_music[m_currentIndex]->stop();
    
        if(m_mode == MusicPlayer::Shuffle)
            return;

        if(m_currentIndex - 1 < m_music.size())
            m_music[--m_currentIndex]->play();
    }
}

unsigned int MusicPlayer::getRandomNumber()
{
    return static_cast<unsigned int>(m_randomGenerator.getValue());
}