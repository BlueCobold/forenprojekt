#pragma once

#ifndef HORIZONTALSTRIPES_TRANSITION_HPP
#define HORIZONTALSTRIPES_TRANSITION_HPP

#include "Transition.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>

class HorizontalStripesTransition : public Transition
{
public:
    
    HorizontalStripesTransition(
        const sf::Texture* sourceTexture,
        const sf::Texture* targetTexture,
        const int stripeCount,
        const float duration);

    void update();

    virtual void draw(const DrawParameter& param);

private:
    sf::Sprite m_sourceSprite;
    std::vector<std::unique_ptr<sf::Sprite>> m_targetSprites;
    int m_stripeHeight;
};

#endif // HORIZONTALSTRIPES_TRANSITION_HPP