#pragma once

#ifndef CAPTIONMENU_HPP
#define CAPTIONMENU_HPP

#include "Menu.hpp"

class CaptionMenu : public Menu
{
public:

    CaptionMenu(const MenuTemplate& menuTemplate,
                const sf::Vector2f& position,
                sf::RenderWindow& screen);

    virtual void draw(const DrawParameter& params);

    virtual void setPosition(const sf::Vector2f& position);

private:

    LineLabel m_caption;

    void rebuild();
};

#endif // CAPTIONMENU_HPP
