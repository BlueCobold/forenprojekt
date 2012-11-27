#pragma once

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "animation/ValueCalculator.hpp"

#include <memory> // unique_ptr

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Animation : sf::Drawable
{
public:

    Animation(std::unique_ptr<ValueCalculator> calculator,
        const unsigned int frameWidth,
        const unsigned int frameHeight,
        const bool applyRotation);
    ~Animation();

    void update();

    void setPosition(const float x, const float y);
    void setRotation(const float radians);
    void bindTexture(const sf::Texture& texture, const sf::Vector2f& offset);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

    const sf::IntRect& Animation::getTextureRect() const;

    std::unique_ptr<ValueCalculator> m_calculator;
    bool m_applyRotation;
    unsigned int m_frame;
    unsigned int m_frameWidth;
    unsigned int m_frameHeight;
    sf::Sprite m_sprite;
};

#endif // ANIMATION_HPP
