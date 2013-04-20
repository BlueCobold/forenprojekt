#include "LoadLevelState.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include "rendering/transitions/RandomTransition.hpp"

#include "gui/HUD.hpp"

#include <iostream>

LoadLevelState::LoadLevelState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config) :
    State(screen, resourceManager, config),
    m_level(nullptr)
{
    m_level = new Level(6, m_resourceManager, m_config);
    bool fromImage = m_texture.create(screen.getSize().x, screen.getSize().y);

    float time = m_frametime.getElapsedTime().asSeconds();
    m_level->restartAt(time);
    m_level->update(time, m_texture);

    HUD hud(resourceManager, config);

    hud.update(m_level, m_frametime.getElapsedTime().asSeconds());

    m_texture.clear();
    m_level->draw(DrawParameter(m_texture));
    hud.draw(m_texture);
    m_texture.display();

    m_transitionStateInfo.m_followingState = PlayStateId;
    m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
    m_transitionStateInfo.m_transition = new RandomTransition(nullptr, &m_texture.getTexture(), 0.5f);

    m_playStateInfo.m_level = m_level;
    m_playStateInfo.m_returnFromPause = false;
}

LoadLevelState::~LoadLevelState()
{

}

void LoadLevelState::onEnter(void* enterInformation)
{
}

StateChangeInformation LoadLevelState::update()
{
    return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
}

void LoadLevelState::draw()
{
}