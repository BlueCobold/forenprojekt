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
#include "InteractiveLabel.hpp"
#include "../model/SoundObject.hpp"
#include "AnimationContainer.hpp"

struct BaseUiInfo {
    BaseUiInfo() : visibleWhenId(-1) { }

    int visibleWhenId;
};

struct ButtonInfo : public BaseUiInfo
{
    ButtonStyle style;
    sf::Vector2f position;
    std::string textResourceKey;
    int id;
    bool triggers;
    ToolTip toolTip;
};

struct CheckBoxInfo : public BaseUiInfo
{
    CheckBoxStyle style;
    sf::Vector2f position;
    int id;
    ToolTip toolTip;
};

struct SliderInfo : public BaseUiInfo
{
    SliderStyle style;
    sf::Vector2f position;
    int id;
};

struct InputBoxInfo : public BaseUiInfo
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
    std::vector<InteractiveLabel> interactiveLabels;
    std::vector<MenuSprite> sprites;
    std::vector<InputBoxInfo> infobox;
    std::vector<std::unique_ptr<AnimationContainer>> animationContainer;

    MenuElements(){};

    MenuElements(const MenuElements& other) :
        buttons(other.buttons),
        checkboxes(other.checkboxes),
        slider(other.slider),
        labels(other.labels),
        sprites(other.sprites),
        infobox(other.infobox),
        interactiveLabels(other.interactiveLabels)
    {
        for(auto it = begin(other.animationContainer); it != end(other.animationContainer); ++it)
            animationContainer.push_back((*it)->clone());
    }

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
        interactiveLabels = std::move(other.interactiveLabels);
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

    SubWindowInfo(const SubWindowInfo& other) :
        menuElements(other.menuElements),
        position(other.position),
        size(other.size),
        innerHeight(other.innerHeight),
        id(other.id),
        style(other.style)
    { }
    
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
    const BitmapFont* captionFont;
    sf::Vector2f captionOffset;
    sf::Sprite background;
    sf::Vector2f relativePosition;
    sf::Vector2f menuOffset;

    MenuTemplate(){}

    MenuTemplate(const MenuTemplate& other) :
        menuElements(other.menuElements),
        subWindow(other.subWindow),
        captionResourceKey(other.captionResourceKey),
        captionFont(other.captionFont),
        captionOffset(other.captionOffset),
        background(other.background),
        relativePosition(other.relativePosition),
        menuOffset(other.menuOffset)
    { }

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
        relativePosition = other.relativePosition;
        menuOffset = other.menuOffset;
    }
};

#endif //MENU_TEMPLATE_HPP
