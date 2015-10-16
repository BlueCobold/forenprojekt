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

    virtual void setCaption(const std::string& caption);

protected:
    
    virtual void drawAdditionalBackground(const DrawParameter& params) override;

private:

    //LineLabel m_caption;

    void rebuild();
};

#endif // CAPTIONMENU_HPP
