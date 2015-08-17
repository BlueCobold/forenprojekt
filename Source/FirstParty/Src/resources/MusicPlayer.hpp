#pragma once

#ifndef MUSIC_PLAYER_HPP
#define MUSIC_PLAYER_HPP

#include <vector>
#include "../animation/provider/RandomProvider.hpp"

namespace sf
{
    class Music;
}

class AppConfig;

class MusicPlayer
{
public:
    enum PlayMode 
    {
        Normal,
        Shuffle
    };
private:
    std::vector<sf::Music*>& m_music;
    RandomProvider m_randomGenerator;
    PlayMode m_mode;
    unsigned int m_currentIndex;
    bool m_isPlaying;
    AppConfig& m_config;
    float m_currentVolume;

    unsigned int getRandomNumber();

public:
    MusicPlayer(AppConfig& config, std::vector<sf::Music*>& music, MusicPlayer::PlayMode mode);

    void update();

    void play();

    void stop();

    void pause();

    void next();

    void previous();
};

#endif