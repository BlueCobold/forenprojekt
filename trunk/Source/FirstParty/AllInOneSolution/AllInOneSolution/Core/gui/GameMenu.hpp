#pragma once

#ifndef GAMEMENU_HPP
#define GAMEMENU_HPP

#include "Menu.hpp"
#include "Button.hpp"

/// Gamemenu
class GameMenu : public Menu
{
public:
    enum ButtonFlag {PlayAgain = 1,
                     NextLevel = 2,
                     MainMenu = 4,
                     Continue = 8};

    //for int Buttons use ButtonFlag!
    GameMenu(sf::Vector2f position, std::string caption, int Buttons, sf::RenderWindow& screen, ResourceManager& resourceManager);

    void draw(const DrawParameter& params);

    void update(const sf::RenderWindow& screen);

    bool isPressed(ButtonFlag button);

    void setPosition(sf::Vector2f position);

    sf::Vector2f getSize();

private:

    int m_activButtons;

    LineLabel m_menuCaption;

    Button m_playAgain;

    Button m_nextLevel;

    Button m_mainMenu;

    Button m_continue;

    sf::Sprite m_menuBackround;

    void rebuild();
};

#endif // GAMEMENU_HPP