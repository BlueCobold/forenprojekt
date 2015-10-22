#pragma once

#ifndef CREDITNMENU_HPP
#define CREDITNMENU_HPP

#include "Menu.hpp"

class CreditMenu : public Menu
{
public:

    static const int BUTTON_CLOSE = 0;

    CreditMenu(sf::RenderWindow& screen, ResourceManager& resourceManager);
};

#endif // CREDITNMENU_HPP