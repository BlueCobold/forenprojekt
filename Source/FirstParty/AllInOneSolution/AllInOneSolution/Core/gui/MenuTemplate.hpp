#pragma once

#ifndef MENU_TEMPLATE_HPP
#define MENU_TEMPLATE_HPP

#include <tinyxml2.h>
#include <string>
#include <vector>

#include "ButtonStyle.hpp"
#include "CheckBoxStyle.hpp"
#include "SliderStyle.hpp"
#include "LineLabel.hpp"
#include "MenuSprite.hpp"
#include "ToolTip.hpp"
#include "InputBoxStyle.hpp"
#include "../model/SoundObject.hpp"
#include "AnimationContainer.hpp"

struct ButtonInfo
{
public:

    ButtonStyle style;
    sf::Vector2f position;
    std::string textResourceKey;
    int id;
    ToolTip toolTip;
};

struct CheckBoxInfo
{
    CheckBoxStyle style;
    sf::Vector2f position;
    int id;
    ToolTip toolTip;
};

struct SliderInfo
{
    SliderStyle style;
    sf::Vector2f position;
    int id;
};

struct InputBoxInfo
{
    InputBoxStyle style;
    sf::Vector2f position;
    sf::Vector2f size;
    unsigned int inputLimit;
    int id;
};

struct MenuElements
{
    std::vector<ButtonInfo> buttons;
    std::vector<CheckBoxInfo> checkboxes;
    std::vector<SliderInfo> slider;
    std::vector<LineLabel> labels;
    std::vector<MenuSprite> sprites;
    std::vector<InputBoxInfo> infobox;
    std::vector<std::unique_ptr<AnimationContainer*>> animationContainer;
};

struct SubWindowStyle
{
    sf::Sprite scrollbarTop;
    sf::Sprite scrollbarMiddle;
    sf::Sprite scrollbarBottom;
};

struct SubWindowInfo
{
    MenuElements menuElements;
    sf::Vector2f position;
    sf::Vector2f size;
    int innerHeight;
    int id;
    SubWindowStyle style;
};

struct MenuTemplate
{
    MenuElements menuElements;
    std::vector<SubWindowInfo> subWindow;
    std::string captionResourceKey;
    BitmapFont* captionFont;
    sf::Vector2f captionOffset;
    sf::Sprite background;
};

#endif //MENU_TEMPLATE_HPP
