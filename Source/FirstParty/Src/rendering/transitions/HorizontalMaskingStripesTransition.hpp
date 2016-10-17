#pragma once

#ifndef HORIZONTALMASKINGSTRIPES_TRANSITION_HPP
#define HORIZONTALMASKINGSTRIPES_TRANSITION_HPP

#include "Transition.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <memory>
#include <vector>

class HorizontalMaskingStripesTransition : public Transition
{
public:
    HorizontalMaskingStripesTransition(
        const sf::Texture* sourceTexture,
        const sf::Texture* targetTexture,
        const int stripeCount,
        const float duration,
        const sf::Vector2u& size);

private:
    sf::Sprite m_sourceSprite;
    std::vector<std::unique_ptr<sf::Sprite>> m_targetSprites;
    int m_stripeWidth;
    int m_stripeHeight;

    void doDraw(const DrawParameter& param);
    void updated() override;
};

#endif // HORIZONTALMASKINGSTRIPES_TRANSITION_HPP
