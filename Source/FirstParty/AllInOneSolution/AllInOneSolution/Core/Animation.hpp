#pragma once

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <SFML/Graphics/Rect.hpp>

class Animation
{
public:

    Animation();
    Animation(const bool infinite, const float min, const float step,
        const unsigned int numFrames, const unsigned int frameWidth, const unsigned int frameHeight);
    ~Animation();

    void start();
    void pause();

    void update(const float value);

    const sf::IntRect& getTextureRect();

private:

    void calculateFrameInfinte(float value);
    void calculateFrame(const float value);

private:

    sf::IntRect m_textureRect;
    float m_min;
    float m_max;
    float m_step;
    unsigned int m_frame;
    unsigned int m_numFrames;
    unsigned int m_frameWidth;
    unsigned int m_frameHeight;
    bool m_infinite;
    bool m_sleep;

};

#endif // ANIMATION_HPP
