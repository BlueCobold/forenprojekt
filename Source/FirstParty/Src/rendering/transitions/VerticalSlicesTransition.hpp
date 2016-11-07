#pragma once

#ifndef VERTICALSLICES_TRANSITION_HPP
#define VERTICALSLICES_TRANSITION_HPP

#include "Transition.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <memory>
#include <vector>

class VerticalSlicesTransition : public Transition
{
public:
    VerticalSlicesTransition(
        const sf::Texture* sourceTexture,
        const sf::Texture* targetTexture,
        const int stripeCount,
        const double duration,
        const sf::Vector2u& size);

private:
    sf::Sprite m_sourceSprite;
    std::vector<std::unique_ptr<sf::Sprite>> m_targetSprites;
    int m_stripeWidth;
    int m_stripeHeight;
    int m_offset;
    float m_timeFactor;

    void doDraw(const DrawParameter& param);
    void updated() override;
};

#endif // VERTICALSLICES_TRANSITION_HPP
