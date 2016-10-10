#pragma once

#ifndef NEWHIGHSCOREMENU_HPP
#define NEWHIGHSCOREMENU_HPP

#include "Menu.hpp"
#include "KeyboardMenu.hpp"

class NewHighScoreMenu : public Menu
{
private:
    KeyboardMenu m_virtualKeyboard;

    void doDraw(const DrawParameter& params) override;

public:
    static const int INPUTBOX = 1;
    static const int BUTTON_SKIP = 5;
    static const int BUTTON_OK = 6;

    NewHighScoreMenu(sf::RenderWindow& screen, ResourceManager& resourceManager);

    void update(sf::RenderWindow& screen, const float time) override;

    void showKeyboard();

    bool isKeyboardShown();
};

#endif // NEWHIGHSCOREMENU_HPP