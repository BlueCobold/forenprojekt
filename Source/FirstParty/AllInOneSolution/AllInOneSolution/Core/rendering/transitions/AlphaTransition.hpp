#pragma once

#ifndef ALPHA_TRANSITION_HPP
#define ALPHA_TRANSITION_HPP

#include "Transition.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>

class AlphaTransition : public Transition
{
public:
    
    AlphaTransition(
        const sf::Texture* sourceTexture,
        const sf::Texture* targetTexture,
        const float duration);

    void update();

    virtual void draw(const DrawParameter& param);

private:
    sf::Sprite m_sourceSprite;
    sf::Sprite m_targetSprite;
};

#endif // ALPHA_TRANSITION_HPP
