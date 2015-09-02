#include "MusicPlayer.hpp"
#include <SFML/Audio/Music.hpp>
#include "../resources/AppConfig.hpp"

MusicPlayer::MusicPlayer(AppConfig& config, std::vector<sf::Music*>& music, MusicPlayer::PlayMode mode) :
    m_config(config),
    m_music(music),
    m_randomGenerator(0.f, static_cast<float>(music.size())),
    m_mode(mode),
    m_isPlaying(false),
    m_currentIndex(0),
    m_currentVolume(0)
{
    if(m_mode == MusicPlayer::Shuffle)
        m_currentIndex = getRandomNumber();

    m_currentVolume = m_config.get<float>("MusicVolume");
}

void MusicPlayer::update()
{
    if(m_config.get<float>("MusicVolume") != m_currentVolume)
    {
        m_currentVolume = m_config.get<float>("MusicVolume");
        m_music[m_currentIndex]->setVolume(m_currentVolume);
    }

    if(!m_isPlaying ||
       m_music[m_currentIndex]->getStatus() == sf::Music::Playing ||
       m_music.empty())
        return;

    next();
}

void MusicPlayer::play()
{
    if(!m_music.empty())
    {
        m_isPlaying = true;
        m_music[m_currentIndex]->play();
        m_music[m_currentIndex]->setVolume(m_currentVolume);
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
        m_music[m_currentIndex]->setVolume(m_currentVolume);
    }
}

void MusicPlayer::previous()
{
    if(!m_music.empty())
    {
        m_isPlaying = true;
        m_music[m_currentIndex]->stop();

        if(m_mode == MusicPlayer::Shuffle)
            next();

        if(m_currentIndex > 0)
            m_music[--m_currentIndex]->play();
        else
            m_music[m_music.size() - 1]->play();
    }
}

unsigned int MusicPlayer::getRandomNumber()
{
    return static_cast<unsigned int>(m_randomGenerator.getValue());
}
