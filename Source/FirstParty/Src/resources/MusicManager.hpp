#pragma once

#ifndef MUSIC_MANAGER_HPP
#define MUSIC_MANAGER_HPP

#include <vector>
#include "../animation/provider/RandomProvider.hpp"

namespace sf
{
    class Music;
}

class MusicManager
{
public:
    enum PlayMode {Normal,
                   Shuffle};
private:
    std::vector<sf::Music*>& m_music;
    RandomProvider m_randomGenerator;
    PlayMode m_mode;
    unsigned int m_currentIndex;
    bool m_isPlaying;

    unsigned int getRandomNumber();

public:
    MusicManager(std::vector<sf::Music*>& music, MusicManager::PlayMode mode);

    void update();

    void play();

    void stop();

    void pause();

    void next();

    void prev();
};

#endif