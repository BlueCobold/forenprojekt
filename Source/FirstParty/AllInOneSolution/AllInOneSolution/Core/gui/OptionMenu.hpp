#pragma once

#ifndef OPTIONMENU_HPP
#define OPTIONMENU_HPP

#include "CaptionMenu.hpp"
#include "../gui/CheckBox.hpp"

class AppConfig;

class OptionMenu : public CaptionMenu
{
public:

    static const int BUTTON_CLOSE = 0;
    static const int CHECKBOX_FULLSCREEN = 1;
    static const int SLIDER_MASTERVOLUMEN = 2;
    static const int CHECKBOX_MUTEINACTIVE = 5;

    OptionMenu(const sf::Vector2f& position, sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config);
    
    void applyChanges();
    void onEnter();
private:

    bool m_fullScreen;
    float m_masterVolume;
    bool m_muteSoundWhenInactive;

    AppConfig& m_config;

    void adjustVideoMode(sf::VideoMode& mode, bool fullScreen);
};

#endif // OPTIONMENU_HPP