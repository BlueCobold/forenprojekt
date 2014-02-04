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
    static const int CHECKBOX_FULLSCREEN = 2;
    static const int SLIDER_MASTERVOLUMEN = 3;
    static const int CHECKBOX_MUTEINACTIVE = 6;
    static const int BUTTON_ARROW_RIGHT = 9;
    static const int BUTTON_ARROW_LEFT = 10;
    static const int LABEL_RESOLUTION = 11;
    static const int CHECKBOX_USE_VERTICALAXIS = 12;
    static const int CHECKBOX_INVERT_AXIS = 15;

    OptionMenu(const sf::Vector2f& position, sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config);
    
    void applyChanges();
    void onEnter();
    void nextVideoMode();
    void prevVideoMode();

private:

    bool m_fullScreen;
    float m_masterVolume;
    bool m_muteSoundWhenInactive;
    bool m_invertAxis;
    bool m_useVerticalAxis;
    std::vector<sf::VideoMode> m_availableVideoMode;
    sf::VideoMode m_currentVideoMode;
    sf::VideoMode m_appointedVideoMode;
    sf::Image m_icon;

    AppConfig& m_config;

    void adjustVideoMode(sf::VideoMode& mode, bool fullScreen);
    bool acceptableVideoMode(const sf::VideoMode videoMode);
    void sortVideoModeList();
};

#endif // OPTIONMENU_HPP