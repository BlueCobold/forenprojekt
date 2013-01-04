#pragma once

#ifndef APP_HPP
#define APP_HPP

#include "resources/ResourceManager.hpp"
#include "StateManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <string>

class Config;

/// This class will be used to handle the window or fullscreen
/// and to handle the rendering
class App
{
public:

    App(Config& config);

    void run();

private:

    void update();
    void draw();

    void handleKeyboard();
    void handleEvents();

    void switchDisplayMode();

    void calculateFps();
    void onResize();

    bool checkVideoMode(const sf::VideoMode mode);

    void adjustVideoMode(sf::VideoMode& mode);

private:

    sf::RenderWindow m_screen;
    sf::Clock m_clock;

    Config& m_config;
    std::string m_windowTitle;
    bool m_fullscreen;
    bool m_focus;

    bool m_showFps;
    int m_fps;
    float m_frameCounter;

    ResourceManager m_resourceManager;
    sf::SoundBuffer* m_soundBuffer;

    sf::Text m_fpsText;

    StateManager m_stateManager;

};

#endif // APP_HPP
