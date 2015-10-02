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

    ButtonStateStyle() :
    font(nullptr),
    animation(nullptr)
    { }

    ButtonStateStyle(const ButtonStateStyle& other) :
        font(other.font),
        spriteOffset(other.spriteOffset),
        textOffset(other.textOffset),
        sprite(other.sprite),
        label(other.label),
        sound(other.sound)
    {
        if(other.animation != nullptr)
            animation = utility::unique_cast<AnimationContainer>(other.animation->clone());
    }

    ButtonStateStyle(ButtonStateStyle&& other)
    {
        font = other.font;
        spriteOffset = other.spriteOffset;
        textOffset = other.textOffset;
        sprite = other.sprite;
        label = other.label;
        sound = other.sound;
        animation = std::move(other.animation);
    }

    ButtonStateStyle& operator= (ButtonStateStyle&& other)
    {
        font = other.font;
        spriteOffset = other.spriteOffset;
        textOffset = other.textOffset;
        sprite = other.sprite;
        label = other.label;
        sound = other.sound;
        animation = std::move(other.animation);
        return *this;
    }

    ButtonStateStyle& operator= (ButtonStateStyle& other)
    {
        font = other.font;
        spriteOffset = other.spriteOffset;
        textOffset = other.textOffset;
        sprite = other.sprite;
        label = other.label;
        sound = other.sound;
        if(other.animation != nullptr)
            animation = utility::unique_cast<AnimationContainer>(other.animation->clone());
        return *this;
    }
};

struct ButtonStyle
{
    ButtonStateStyle idleStyle;
    ButtonStateStyle hoverStyle;
    ButtonStateStyle pressedStyle;
    sf::IntRect mouseRect;
};

#endif //BUTTON_STYLE_HPP
