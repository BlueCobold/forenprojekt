#pragma once

#ifndef CAPTIONMENU_HPP
#define CAPTIONMENU_HPP

#include "Menu.hpp"

class CaptionMenu : public Menu
{
public:

    CaptionMenu(const sf::Vector2f& position, 
                const std::string&, 
                sf::RenderWindow& screen, 
                ResourceManager& resourceManager,
                const std::string& backgroundName);

    virtual void draw(const DrawParameter& params);

    virtual void setPosition(const sf::Vector2f& position);

private:

    LineLabel m_caption;

    void rebuild();
};

#endif // CAPTIONMENU_HPP
