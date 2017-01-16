#pragma once

#ifndef BORDER_STYLE_HPP
#define BORDER_STYLE_HPP

#include "ScreenSize.hpp"
#include "../rendering/Sprite.hpp"

#include <SFML/Graphics/Rect.hpp>

#include <array>
#include <unordered_map>
#include <utility>

struct BorderStyle
{
    enum BackgroundId : int
    {
        TopLeft = 1,
        TopCenter = 2,
        TopRight = 3,
        MiddleLeft = 4,
        MiddleRight = 5,
        BottomLeft = 6,
        BottomCenter = 7,
        BottomRight = 8,
        Background = 9
    };

    enum DecoId : int
    {
        Top = 0,
        Right = 1,
        Left = 2,
        Bottom = 3
    };
    
    std::unordered_map<BackgroundId, Sprite, std::hash<int>> backgrounds;
    std::array<std::vector<std::pair<Sprite, sf::Vector2f>>, 4> decos;
    sf::FloatRect innerOffsets;
};

#endif // BORDER_STYLE_HPP
