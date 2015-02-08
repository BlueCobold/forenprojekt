#pragma once

#ifndef NEWHIGHSCOREMENU_HPP
#define NEWHIGHSCOREMENU_HPP

#include "CaptionMenu.hpp"
#include "KeyboardMenu.hpp"

class NewHighScoreMenu : public Menu
{
private:
    KeyboardMenu m_virtualKeyboard;
public:
    static const int INPUTBOX = 1;
    static const int BUTTON_SKIP = 5;
    static const int BUTTON_OK = 6;

    NewHighScoreMenu(const sf::Vector2f& position, sf::RenderWindow& screen, ResourceManager& resourceManager);

    virtual void draw(const DrawParameter& params) override;
    virtual void update(sf::RenderWindow& screen, const float time) override;

    void showKeyboard();
};

#endif // NEWHIGHSCOREMENU_HPP