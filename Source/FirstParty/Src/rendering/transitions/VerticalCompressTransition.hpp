#pragma once

#ifndef VERTICALCOMPRESS_TRANSITION_HPP
#define VERTICALCOMPRESS_TRANSITION_HPP

#include "Transition.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <memory>
#include <vector>

class VerticalCompressTransition : public Transition
{
public:
    VerticalCompressTransition(
        const sf::Texture* sourceTexture,
        const sf::Texture* targetTexture,
        const int stripeCount,
        const double duration,
        const sf::Vector2u& size);

private:
    std::vector<std::unique_ptr<sf::Sprite>> m_sourceSprites;
    std::vector<std::unique_ptr<sf::Sprite>> m_targetSprites;
    int m_stripeWidth;
    int m_stripeHeight;

    void doDraw(const DrawParameter& param) override;
    void updated() override;
};

#endif // VERTICALCOMPRESS_TRANSITION_HPP
