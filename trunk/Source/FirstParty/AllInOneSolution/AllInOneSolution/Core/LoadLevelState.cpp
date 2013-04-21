#include "LoadLevelState.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include "rendering/transitions/RandomTransition.hpp"

#include <memory>

LoadLevelState::LoadLevelState(sf::RenderWindow& screen, ResourceManager& resourceManager, Config& config) :
    State(screen, resourceManager, config),
    m_level(nullptr),
    m_hud(new HUD(resourceManager, config))
{
}

LoadLevelState::~LoadLevelState()
{

}

void LoadLevelState::onEnter(void* enterInformation)
{
}

StateChangeInformation LoadLevelState::update()
{
    m_level = std::unique_ptr<Level>(new Level(6, m_resourceManager, m_config));
    bool fromImage = m_texture.create(m_screen.getSize().x, m_screen.getSize().y);

    float time = m_frametime.getElapsedTime().asSeconds();
    m_level->restartAt(time);
    m_level->update(time, m_texture);

    m_hud->update(m_level.get(), m_frametime.getElapsedTime().asSeconds());

    m_texture.clear();
    m_level->draw(DrawParameter(m_texture));
    m_hud->draw(m_texture);
    m_texture.display();

    m_transitionStateInfo.m_followingState = PlayStateId;
    m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
    std::unique_ptr<Transition> state = std::unique_ptr<RandomTransition>(
        new RandomTransition(nullptr, &m_texture.getTexture(), 0.5f, m_screen.getSize()));
    m_transitionStateInfo.m_transition = std::move(state);

    m_playStateInfo.m_level = std::move(m_level);
    m_playStateInfo.m_returnFromPause = false;

    return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
}

void LoadLevelState::draw()
{
}