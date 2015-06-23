#pragma once

#ifndef APP_HPP
#define APP_HPP

#include "resources/ResourceManager.hpp"
#include "resources/AchievementManager.hpp"
#include "State/StateManager.hpp"

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/System/Clock.hpp>

#include <string>
#include <map>

class Cursor;
class AppConfig;

/// This class will be used to handle the window or fullscreen
/// and to handle the rendering
class App : public ResourceManager::ShaderContext
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
    
    virtual float getValueOf(const std::string& name) const override;
    virtual void setValueOf(const std::string& name, const float value) override;

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
    sf::Clock m_clock;
    std::map<std::string, float> m_variables;

    ResourceManager m_resourceManager;
    StateManager m_stateManager;
    AchievementManager m_achievementManager;
};

#endif // APP_HPP
