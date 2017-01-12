#pragma once

#ifndef VERTICALSTRIPES_TRANSITION_HPP
#define VERTICALSTRIPES_TRANSITION_HPP

#include "Transition.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <memory>
#include <vector>

class VerticalStripesTransition : public Transition
{
public:
    VerticalStripesTransition(
        const sf::Texture* sourceTexture,
        const sf::Texture* targetTexture,
        const int stripeCount,
        const double duration,
        const sf::Vector2u& size);

private:
    sf::Sprite m_sourceSprite;
    std::vector<std::unique_ptr<sf::Sprite>> m_targetSprites;
    int m_stripeWidth;

    void doDraw(const DrawParameter& param) override;
    void updated() override;
};

#endif // ALPHA_TRANSITION_HPP
