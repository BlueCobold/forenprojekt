#pragma once

#ifndef SLIDER_STYLE_HPP
#define SLIDER_STYLE_HPP

#include "BitmapFont.hpp"
#include "LineLabel.hpp"
#include "../rendering/Sprite.hpp"

#include <SFML/System/Vector2.hpp>

struct SliderStateStyle
{
    sf::Vector2f backgroundOffset;
    sf::Vector2f buttonOffset;
    Sprite background;
    Sprite button;
};

struct SliderStyle
{
    SliderStateStyle idle;
    SliderStateStyle active;
    sf::IntRect mouseRect;
    float min;
    float max;
    float width;
};
#endif //SLIDER_STYLE_HPP
