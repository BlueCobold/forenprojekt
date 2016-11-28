#pragma once

#ifndef BLENDING_HPP
#define BLENDING_HPP

#include <SFML/Graphics/BlendMode.hpp>

struct Blending
{
    enum Mode
    {
        RegularAlpha,
        PreMultipliedAlpha,
        Multiply,
        Add
    };

    static sf::BlendMode toSfmlMode(const Mode mode) 
    {
        switch (mode)
        {
            case RegularAlpha:
                return sf::BlendAlpha;
            case PreMultipliedAlpha:
                return sf::BlendMode(sf::BlendMode::One, sf::BlendMode::OneMinusSrcAlpha);
            case Multiply:
                return sf::BlendMultiply;
            case Add:
                return sf::BlendAdd;
        }
        return sf::BlendAlpha;
    }
};

#endif // SPRITE_HPP
