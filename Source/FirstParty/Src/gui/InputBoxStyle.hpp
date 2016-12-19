#pragma once

#ifndef INPUTBOXSTYLE_HPP
#define INPUTBOXSTYLE_HPP

#include "BitmapFont.hpp"
#include "../rendering/Sprite.hpp"

#include <SFML/System/Vector2.hpp>

#include <unordered_map>

struct InputBoxStyle
{
    const BitmapFont* font;
    sf::Vector2f textOffset;
    std::unordered_map<int, Sprite> background;
    Sprite caret;
    sf::Vector2f caretOffset;
    float caretBlinkFrequency;
};

#endif // INPUTBOXSTYLE_HPP