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
    std::vector<std::unique_ptr<AnimationContainer>> animationContainer;

    MenuElements(){};

    MenuElements& operator= (MenuElements&& other)
    {
        move(other);
        return *this;
    }

    MenuElements(MenuElements&& other)
    {
        move(other);
    }

private:
    void move(MenuElements& other)
    {
        buttons = std::move(other.buttons);
        checkboxes = std::move(other.checkboxes);
        slider = std::move(other.slider);
        labels = std::move(other.labels);
        sprites = std::move(other.sprites);
        infobox = std::move(other.infobox);
        animationContainer = std::move(other.animationContainer);
    }
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

    SubWindowInfo(){};

    SubWindowInfo(SubWindowInfo&& other)
    {
        move(other);
    }

    SubWindowInfo& operator= (SubWindowInfo&& other)
    {
        move(other);
        return *this;
    }

private:
    void move(SubWindowInfo& other)
    {
        menuElements = std::move(other.menuElements);
        position = other.position;
        size = other.size;
        innerHeight = other.innerHeight;
        id = other.id;
        style = other.style;
    }
};

struct MenuTemplate
{
    MenuElements menuElements;
    std::vector<SubWindowInfo> subWindow;
    std::string captionResourceKey;
    BitmapFont* captionFont;
    sf::Vector2f captionOffset;
    sf::Sprite background;

    MenuTemplate(){}
    
    MenuTemplate(MenuTemplate&& other)
    {
        move(other);
    }

    MenuTemplate& operator= (MenuTemplate&& other)
    {
        move(other);
        return *this;
    }

private:
    void move(MenuTemplate& other)
    {
        menuElements = std::move(other.menuElements);
        subWindow = std::move(other.subWindow);
        captionResourceKey = other.captionResourceKey;
        captionFont = other.captionFont;
        captionOffset = other.captionOffset;
        background = other.background;
    }
};

#endif //MENU_TEMPLATE_HPP
