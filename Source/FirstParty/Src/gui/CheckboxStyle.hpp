#pragma once

#ifndef CHECKBOX_STYLE_HPP
#define CHECKBOX_STYLE_HPP

#include "BitmapFont.hpp"
#include "LineLabel.hpp"

#include "../rendering/Sprite.hpp"

#include <SFML/System/Vector2.hpp>

struct CheckBoxStateStyle
{
    sf::Vector2f spriteOffset;
    Sprite sprite;
};

struct CheckBoxStyle
{
    CheckBoxStateStyle uncheckedStyle;
    CheckBoxStateStyle checkedStyle;
    CheckBoxStateStyle hoverStyle;
    CheckBoxStateStyle checkedHoverStyle;
    sf::IntRect mouseRect;
};

#endif //CHECKBOX_STYLE_HPP