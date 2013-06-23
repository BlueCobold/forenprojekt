#pragma once

#ifndef SLIDER_STYLE_HPP
#define SLIDER_STYLE_HPP

#include "BitmapFont.hpp"
#include "LineLabel.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>

struct SliderStateStyle
{
    BitmapFont* font;
    sf::Vector2f spriteOffset;
    sf::Vector2f textOffset;
    sf::Sprite spriteBackround;
    sf::Sprite spriteSlider;
    LineLabel label;
};

struct SliderStyle
{
    SliderStateStyle unselected;
    SliderStateStyle selected;
    sf::IntRect mouseRect;
    float min;
    float max;
};
#endif //SLIDER_STYLE_HPP