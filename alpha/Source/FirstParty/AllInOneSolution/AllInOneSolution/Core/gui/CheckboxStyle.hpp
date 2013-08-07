#pragma once

#ifndef CHECKBOX_STYLE_HPP
#define CHECKBOX_STYLE_HPP

#include "BitmapFont.hpp"
#include "LineLabel.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>

struct CheckBoxStateStyle
{
    sf::Vector2f spriteOffset;
    sf::Sprite sprite;
};

struct CheckBoxStyle
{
    CheckBoxStateStyle uncheckedStyle;
    CheckBoxStateStyle checkedStyle;
    sf::IntRect mouseRect;
};

#endif //CHECKBOX_STYLE_HPP