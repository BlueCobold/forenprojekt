#pragma once

#ifndef MENU_TEMPLATE_HPP
#define MENU_TEMPLATE_HPP

#include "ButtonStyle.hpp"

#include <tinyxml2.h>

#include <string>
#include <vector>

struct ButtonInfo
{
    ButtonStyle style;
    sf::Vector2f position;
    std::string textResourceKey;
    int id;
};

struct MenuTemplate
{
    std::vector<ButtonInfo> buttons;
    std::string captionResourceKey;
    BitmapFont* captionFont;
    sf::Vector2f captionOffset;
    sf::Sprite background;
};

#endif //MENU_TEMPLATE_HPP
