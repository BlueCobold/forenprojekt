#pragma once

#ifndef INPUTBOXSTYLE_HPP
#define INPUTBOXSTYLE_HPP

#include "BitmapFont.hpp"
#include <unordered_map>

struct InputBoxStyle
{
    const BitmapFont* font;
    sf::Vector2f textOffset;
    std::unordered_map<int, sf::Sprite> background;
    sf::Sprite caret;
    sf::Vector2f caretOffset;
    float caretBlinkFrequency;
};

#endif // INPUTBOXSTYLE_HPP