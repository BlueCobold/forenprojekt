#pragma once

#ifndef INPUTBOXSTYLE_HPP
#define INPUTBOXSTYLE_HPP

#include "BitmapFont.hpp"
#include <unordered_map>

struct InputBoxStyle
{
    BitmapFont* font;
    sf::Vector2f textOffset;
    std::unordered_map<int, sf::Sprite> background;
};

#endif // INPUTBOXSTYLE_HPP