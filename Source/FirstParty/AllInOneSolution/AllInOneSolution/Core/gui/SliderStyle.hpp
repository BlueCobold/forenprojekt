#pragma once

#ifndef SLIDER_STYLE_HPP
#define SLIDER_STYLE_HPP

#include "BitmapFont.hpp"
#include "LineLabel.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>

struct SliderStateStyle
{
    sf::Vector2f spriteOffset;
    sf::Sprite spriteBackround;
    sf::Sprite spriteSlider;
};

struct SliderStyle
{
    SliderStateStyle idle;
    SliderStateStyle active;
    sf::IntRect mouseRect;
    float min;
    float max;
};
#endif //SLIDER_STYLE_HPP