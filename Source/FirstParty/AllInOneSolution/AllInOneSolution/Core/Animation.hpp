#pragma once

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "animation/ValueCalculator.hpp"

#include <memory> // unique_ptr
#include <SFML/Graphics/Rect.hpp>

class Animation
{
public:

    Animation(std::unique_ptr<ValueCalculator> calculator,
        const unsigned int frameWidth,
        const unsigned int frameHeight);
    ~Animation();

    void update();

    const sf::IntRect& getTextureRect();

private:

    std::unique_ptr<ValueCalculator> m_calculator;
    sf::IntRect m_textureRect;
    unsigned int m_frame;
    unsigned int m_frameWidth;
    unsigned int m_frameHeight;

};

#endif // ANIMATION_HPP
