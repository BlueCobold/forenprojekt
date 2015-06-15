#pragma once

#ifndef APP_HPP
#define APP_HPP

#include "resources/ResourceManager.hpp"
#include "resources/AchievementManager.hpp"
#include "Utility.hpp" // toString
#include "State/StateManager.hpp"
#include "gui/Cursor.hpp"
#include "gui/LineLabel.hpp"
#include "resources/AppConfig.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Image.hpp>
#include "gui/ErrorMessageBox.hpp"
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

    void minimize();
    void restore();

private:
    sf::RenderWindow m_screen;
    std::array<sf::RenderTexture, 2> m_offscreens;
    std::unique_ptr<Cursor> m_cursor;

    AppConfig& m_config;
    std::string m_windowTitle;
    bool m_fullscreen;
    bool m_focus;
    bool m_isMinimized;

    utility::Event m_event;

    ResourceManager m_resourceManager;
    StateManager m_stateManager;
    AchievementManager m_achievementManager;
};

#endif // APP_HPP
