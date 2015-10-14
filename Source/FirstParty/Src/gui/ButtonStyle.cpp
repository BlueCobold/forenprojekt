#include "ButtonStyle.hpp"

ButtonStateStyle::ButtonStateStyle() :
    font(nullptr),
    animation(nullptr),
    resetOnExit(false)
{ }

ButtonStateStyle::ButtonStateStyle(const ButtonStateStyle& other) :
    animation(other.animation ? utility::unique_cast<AnimationContainer>(other.animation->clone()) : nullptr)
{
    copyValues(other);
}

ButtonStateStyle::ButtonStateStyle(ButtonStateStyle&& other) :
    animation(std::move(other.animation))
{
    copyValues(other);
}

ButtonStateStyle& ButtonStateStyle::operator= (ButtonStateStyle&& other)
{
    copyValues(other);
    animation = std::move(other.animation);
    return *this;
}

void ButtonStateStyle::copyValues(const ButtonStateStyle& other)
{
    font = other.font;
    spriteOffset = other.spriteOffset;
    textOffset = other.textOffset;
    sprite = other.sprite;
    label = other.label;
    sound = other.sound;
}

ButtonStyle::ButtonStyle()
{ }

ButtonStyle::ButtonStyle(const ButtonStyle& other) :
    idleStyle(other.idleStyle),
    hoverStyle(other.hoverStyle),
    pressedStyle(other.pressedStyle),
    mouseRect(other.mouseRect)
{ }

ButtonStyle::ButtonStyle(ButtonStyle&& other) :
    idleStyle(std::move(other.idleStyle)),
    hoverStyle(std::move(other.hoverStyle)),
    pressedStyle(std::move(other.pressedStyle)),
    mouseRect(other.mouseRect)
{ }

ButtonStyle& ButtonStyle::operator= (ButtonStyle&& other)
{
    idleStyle = std::move(other.idleStyle);
    hoverStyle = std::move(other.hoverStyle);
    pressedStyle = std::move(other.pressedStyle);
    mouseRect = other.mouseRect;
    return *this;
}
