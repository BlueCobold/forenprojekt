#pragma once

#ifndef APP_HPP
#define APP_HPP

#include "gui/Cursor.hpp"
#include "gui/GestureHandler.hpp"
#include "model/Level.hpp"
#include "resources/AppConfig.hpp"
#include "resources/ResourceManager.hpp"
#include "resources/AchievementManager.hpp"
#include "resources/MusicPlayer.hpp"
#include "State/StateManager.hpp"

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/System/Clock.hpp>

#include <string>
#include <map>

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

    void minimize();
    void restore();

private:
    AppConfig& m_config;
    std::string m_windowTitle;
    sf::RenderWindow m_screen;
    GestureHandler m_gestures;
    bool m_fullscreen;
    bool m_focus;
    bool m_isMinimized;

    std::array<sf::RenderTexture, 2> m_offscreens;
    std::unique_ptr<Cursor> m_cursor;
    std::unique_ptr<Level> m_currentLevel;

    utility::Event m_event;
    ShaderContext m_shaderContext;

    ResourceManager m_resourceManager;
    StateManager m_stateManager;
    AchievementManager m_achievementManager;
    MusicPlayer m_musicPlayer;
    sf::Clock m_frameClock;
};

#endif // APP_HPP
