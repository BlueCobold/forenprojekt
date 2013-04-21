#pragma once

#ifndef GROWINGRECTANGLE_TRANSITION_HPP
#define GROWINGRECTANGLE_TRANSITION_HPP

#include "Transition.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>

class GrowingRectangleTransition : public Transition
{
public:
    
    GrowingRectangleTransition(
        const sf::Texture* sourceTexture,
        const sf::Texture* targetTexture,
        const float duration,
        const sf::Vector2u& size);

    void update();

    virtual void draw(const DrawParameter& param);

private:
    sf::Sprite m_sourceSprite;
    sf::Sprite m_targetSprites;
    int m_RectangleStartX;
    int m_RectangleStartY;
};

#endif // GROWINGRECTANGLE_TRANSITION_HPP