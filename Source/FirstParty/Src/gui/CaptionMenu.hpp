#pragma once

#ifndef CAPTIONMENU_HPP
#define CAPTIONMENU_HPP

#include "LineLabel.hpp"
#include "Menu.hpp"

class CaptionMenu : public Menu
{
public:

    CaptionMenu(const MenuTemplate& menuTemplate,
                sf::RenderWindow& screen);

    virtual void setPosition(const sf::Vector2f& relativePosition,
                             const sf::Vector2f& offset) override;

    virtual void setCaption(const std::string& caption);
    virtual void updateLayout() override;

protected:
    
    virtual void drawAdditionalBackground(const DrawParameter& params) override;

private:

    LineLabel m_caption;

    void rebuild();
};

#endif // CAPTIONMENU_HPP
