#pragma once

#ifndef CAPTIONMENU_HPP
#define CAPTIONMENU_HPP

#include "Menu.hpp"

class CaptionMenu : public Menu
{
public:

    CaptionMenu(MenuTemplate& menuTemplate,
                const sf::Vector2f& position,
                sf::RenderWindow& screen);

    virtual void setPosition(const sf::Vector2f& position) override;
    virtual void setCaption(const std::string& caption);
    virtual void updateLayout() override;

protected:
    
    virtual void drawAdditionalBackground(const DrawParameter& params) override;

private:

    LineLabel m_caption;

    void rebuild();
};

#endif // CAPTIONMENU_HPP
