#pragma once

#ifndef BUTTON_STYLE_HPP
#define BUTTON_STYLE_HPP

#include "BitmapFont.hpp"
#include "LineLabel.hpp"
#include "AnimationContainer.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>

class SoundObject;

struct ButtonStateStyle
{
    const BitmapFont* font;
    sf::Vector2f spriteOffset;
    sf::Vector2f textOffset;
    sf::Sprite sprite;
    LineLabel label;
    std::shared_ptr<SoundObject> sound;
    std::unique_ptr<AnimationContainer> animation;

    ButtonStateStyle();
    ButtonStateStyle(const ButtonStateStyle& other);
    ButtonStateStyle(ButtonStateStyle&& other);

    ButtonStateStyle& operator= (ButtonStateStyle&& other);

private:
    void copyValues(const ButtonStateStyle& other);
};

struct ButtonStyle
{
    ButtonStateStyle idleStyle;
    ButtonStateStyle hoverStyle;
    ButtonStateStyle pressedStyle;
    sf::IntRect mouseRect;
    
    ButtonStyle();
    ButtonStyle(const ButtonStyle& other);
    ButtonStyle(ButtonStyle&& other);

    ButtonStyle& operator= (ButtonStyle&& other);
};

#endif //BUTTON_STYLE_HPP
