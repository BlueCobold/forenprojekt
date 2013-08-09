#pragma once

#ifndef APP_HPP
#define APP_HPP

#include "resources/ResourceManager.hpp"
#include "Utility.hpp" // toString
#include "State/StateManager.hpp"
#include "gui/LineLabel.hpp"
#include "resources/AppConfig.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Image.hpp>

#include <string>

class AppConfig;

/// This class will be used to handle the window or fullscreen
/// and to handle the rendering
class App
{
public:

    App(AppConfig& config);

    void run();

private:

    void update();
    void draw();

    void handleKeyboard();
    void handleEvents();

    void switchDisplayMode();

    void onResize();

    void adjustVideoMode(sf::VideoMode& mode);

private:
    sf::RenderWindow m_screen;

    AppConfig& m_config;
    std::string m_windowTitle;
    sf::Image m_windowIcon;
    bool m_fullscreen;
    bool m_focus;

    utility::Event m_event;

    ResourceManager m_resourceManager;
    StateManager m_stateManager;
};

#endif // APP_HPP
