#pragma once

#ifndef HORIZONTALHALVES_TRANSITION_HPP
#define HORIZONTALHALVES_TRANSITION_HPP

#include "Transition.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>

class HorizontalHalvesTransition : public Transition
{
public:
    
    HorizontalHalvesTransition(
        const sf::Texture* sourceTexture,
        const sf::Texture* targetTexture,
        const int stripeCount,
        const float duration,
        const sf::Vector2u& size);

    void update();

    virtual void draw(const DrawParameter& param);

private:
    sf::Sprite m_targetSprite;
    std::vector<std::unique_ptr<sf::Sprite>> m_sourceSprites;
    int m_stripeWidth;
    int m_stripeHeight;
};

#endif // HORIZONTALHALVES_TRANSITION_HPP