#pragma once

#ifndef CREDITNMENU_HPP
#define CREDITNMENU_HPP

#include "CaptionMenu.hpp"
#include "../gui/CheckBox.hpp"

class Config;

class CreditMenu : public CaptionMenu
{
public:

    static const int BUTTON_CLOSE = 0;

    CreditMenu(const sf::Vector2f& position, sf::RenderWindow& screen, ResourceManager& resourceManager);
};

#endif // CREDITNMENU_HPP