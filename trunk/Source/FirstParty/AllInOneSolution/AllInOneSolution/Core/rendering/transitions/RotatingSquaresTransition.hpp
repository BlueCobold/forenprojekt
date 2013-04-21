#pragma once

#ifndef ROTATINGSQUARES_TRANSITION_HPP
#define ROTATINGSQUARES_TRANSITION_HPP

#include "Transition.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>

class RotatingSquaresTransition : public Transition
{
public:
    
    RotatingSquaresTransition(
        const sf::Texture* sourceTexture,
        const sf::Texture* targetTexture,
        const int columns,
        const int rows,
        const float duration,
        const sf::Vector2u& size);

    void update();

    virtual void draw(const DrawParameter& param);

private:
    sf::Sprite m_sourceSprite;
    std::vector<std::unique_ptr<sf::Sprite>> m_targetSprites;
    int m_rectWidth;
    int m_rectHeight;
    int m_columns;
    int m_rows;
};

#endif // ROTATINGSQUARES_TRANSITION_HPP