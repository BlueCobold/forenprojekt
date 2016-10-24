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

    MusicPlayer(AppConfig& config, std::vector<sf::Music*>& music, MusicPlayer::PlayMode mode);

    void update();
    void play();
    void stop();
    void pause();
    void next();
    void previous();

private:
    AppConfig& m_config;
    std::vector<sf::Music*>& m_music;
    RandomProvider m_randomGenerator;
    PlayMode m_mode;
    bool m_isPlaying;
    unsigned int m_currentIndex;
    float m_currentVolume;

    unsigned int getRandomNumber();
};

#endif
