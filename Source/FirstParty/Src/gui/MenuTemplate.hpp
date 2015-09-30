#pragma once

#ifndef MENU_TEMPLATE_HPP
#define MENU_TEMPLATE_HPP

#include <tinyxml2.h>

#include <memory>
#include <string>
#include <vector>

#include <SFML/Graphics/Sprite.hpp>

#include "MenuElement.hpp"
#include "BitmapFont.hpp"

struct MenuTemplate
{
    std::vector<std::unique_ptr<MenuElement>> menuElements;
    std::string captionResourceKey;
    const BitmapFont* captionFont;
    sf::Vector2f captionOffset;
    sf::Sprite background;
    sf::Vector2f relativePosition;
    sf::Vector2f menuOffset;

    MenuTemplate()
    { }

    MenuTemplate(const MenuTemplate& other) :
        captionResourceKey(other.captionResourceKey),
        captionFont(other.captionFont),
        captionOffset(other.captionOffset),
        background(other.background),
        relativePosition(other.relativePosition),
        menuOffset(other.menuOffset)
    {
        for(auto it = begin(other.menuElements); it != end(other.menuElements); ++it)
            menuElements.push_back((*it)->clone());
    }

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
        captionResourceKey = other.captionResourceKey;
        captionFont = other.captionFont;
        captionOffset = other.captionOffset;
        background = other.background;
        relativePosition = other.relativePosition;
        menuOffset = other.menuOffset;
    }
};

#endif //MENU_TEMPLATE_HPP
