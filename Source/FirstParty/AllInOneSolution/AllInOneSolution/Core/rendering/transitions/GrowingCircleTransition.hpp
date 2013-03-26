#pragma once

#ifndef GROWINGCIRCLE_TRANSITION_HPP
#define GROWINGCIRCLE_TRANSITION_HPP

#include "Transition.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/CircleShape.hpp>

class GrowingCircleTransition : public Transition
{
public:
    
    GrowingCircleTransition(
        const sf::Texture* sourceTexture,
        const sf::Texture* targetTexture,
        const float duration);

    void update();

    virtual void draw(const DrawParameter& param);

private:
    sf::Sprite m_sourceSprite;
    sf::CircleShape m_targetSprites;
    int m_RectangleStartX;
    int m_RectangleStartY;
    float m_radius;
};

#endif // GROWINGCIRCLE_TRANSITION_HPP