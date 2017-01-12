#pragma once

#ifndef VERTICALSPRING_TRANSITION_HPP
#define VERTICALSPRING_TRANSITION_HPP

#include "Transition.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <memory>
#include <vector>

class VerticalSpringTransition : public Transition
{
public:
    VerticalSpringTransition(
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

#endif // VERTICALSPRING_TRANSITION_HPP
