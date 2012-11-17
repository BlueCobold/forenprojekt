#pragma once

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <SFML/Graphics/Rect.hpp>

class Animation
{
public:

    Animation();
    Animation(const bool infinite, const float min, const float max, const float step, const unsigned int numFrames
        , const unsigned int frameWidth, const unsigned int frameHeight);

    ~Animation();

    void start()
    {
        m_sleep = false;
    }

    void pause()
    {
        m_sleep = true;
    }

    void update(const float value);

    sf::IntRect getTextureRect()
    {
        return m_textureRect;
    }

private:

    void calculateFrameInfinte(float value);
    void calculateFrame(const float value);

private:

    sf::IntRect m_textureRect;
    float m_min, m_max, m_step;
    unsigned int m_frame, m_numFrames;
    unsigned int m_frameWidth, m_frameHeight;
    bool m_infinite;
    bool m_sleep;

};

#endif // ANIMATION_HPP
