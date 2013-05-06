#include "LoadLevelState.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include "rendering/transitions/RandomTransition.hpp"

#include <memory>

LoadLevelState::LoadLevelState(sf::RenderWindow& screen, 
                               ResourceManager& resourceManager, 
                               Config& config) :
    State(screen, resourceManager, config),
    m_level(nullptr),
    m_lastLevel(nullptr)
{
}

LoadLevelState::~LoadLevelState()
{

}

void LoadLevelState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    State::onEnter(enterInformation, time);
}

std::unique_ptr<Level> LoadLevelState::gainLevel()
{
    return std::move(m_level);
}

StateChangeInformation LoadLevelState::update(const float time)
{
    updateTime(time);
    m_level = std::unique_ptr<Level>(new Level(7, m_resourceManager, m_config));
    m_lastLevel = m_level.get();
    
    m_playStateInfo.m_returnFromPause = false;
    m_playStateInfo.m_level = m_lastLevel;
    m_transitionStateInfo.m_level = m_lastLevel;
    m_transitionStateInfo.m_followingState = PlayStateId;
    m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;

    return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
}

void LoadLevelState::draw(const DrawParameter& params)
{
}
