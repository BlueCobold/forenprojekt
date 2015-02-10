#pragma once

#ifndef GAMEFINISHEDMENU_HPP
#define GAMEFINISHEDMENU_HPP

#include "Menu.hpp"

class GameFinishedMenu : public Menu
{
public:

    static const int BUTTON_CLOSE = 0;

    GameFinishedMenu(sf::RenderWindow& screen, ResourceManager& resourceManager);
};

#endif // CREDITNMENU_HPP