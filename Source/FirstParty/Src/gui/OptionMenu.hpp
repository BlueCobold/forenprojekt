#pragma once

#ifndef OPTIONMENU_HPP
#define OPTIONMENU_HPP

#include "Menu.hpp"
#include "../gui/CheckBox.hpp"

class AppConfig;

class OptionMenu : public Menu
{
public:

    static const int BUTTON_CLOSE = 0;
    static const int CHECKBOX_FULLSCREEN = 2;
    static const int SLIDER_SOUNDVOLUMEN = 3;
    static const int SLIDER_MUSICVOLUMEN = 33;
    static const int CHECKBOX_MUTEINACTIVE = 6;
    static const int BUTTON_ARROW_RIGHT = 9;
    static const int BUTTON_ARROW_LEFT = 10;
    static const int LABEL_RESOLUTION = 11;
    static const int CHECKBOX_USE_VERTICALAXIS = 12;
    static const int CHECKBOX_INVERT_AXIS = 15;
    static const int CHECKBOX_USE_SHADER_EFFECTS = 30;
    static const int CHECKBOX_USE_STENCIL_EFFECTS = 35;
    static const int CHECKBOX_SHOW_BATTERY_STATE = 38;
    static const int CHECKBOX_LANGUAGE = 41;

    OptionMenu(sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config);

    void applyChanges(sf::RenderWindow& screen);
    void onEnter(const sf::RenderWindow& screen);
    void nextVideoMode();
    void prevVideoMode();

private:

    AppConfig& m_config;

    bool m_fullScreen;
    float m_musicVolume;
    float m_soundVolume;
    bool m_muteSoundWhenInactive;
    bool m_invertAxis;
    bool m_useVerticalAxis;
    bool m_useStencilEffects;
    bool m_useShaderEffects;
    std::vector<sf::Vector2u> m_availableVideoMode;
    sf::Vector2u m_currentVideoMode;
    sf::Vector2u m_appointedVideoMode;
    int m_currentVideoModeIndex;
    sf::Image m_icon;
    bool m_showBatteryState;

    void adjustVideoMode(sf::VideoMode& mode, bool fullScreen);
    bool acceptableVideoMode(const sf::VideoMode videoMode);
    void sortVideoModeList();
};
#endif // OPTIONMENU_HPP
