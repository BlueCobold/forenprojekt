#pragma once

#ifndef BUTTON_STYLE_HPP
#define BUTTON_STYLE_HPP

#include "BitmapFont.hpp"
#include "LineLabel.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>

struct ButtonStateStyle
{
    BitmapFont* font;
    sf::Vector2f spriteOffset;
    sf::Vector2f textOffset;
    sf::Sprite sprite;
    LineLabel label;
};

struct ButtonStyle
{
    ButtonStateStyle idleStyle;
    ButtonStateStyle hoverStyle;
    ButtonStateStyle pressedStyle;
    sf::IntRect mouseRect;
};

#endif //BUTTON_STYLE_HPP
