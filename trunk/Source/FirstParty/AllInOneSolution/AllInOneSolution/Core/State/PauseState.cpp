#include "PauseState.hpp"

#include "../gui/Button.hpp"
#include "../Input.hpp"
#include "../model/Level.hpp"
#include "../resources/Config.hpp"
#include "../resources/ResourceManager.hpp"
#include "../rendering/transitions/RandomTransition.hpp"

#include <memory>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>

PauseState::PauseState(sf::RenderWindow& screen,
                       ResourceManager& resourceManager,
                       AppConfig& config) :
    State(screen, resourceManager, config),
    m_background(nullptr),
    m_menu(sf::Vector2f(0, 0), screen, resourceManager),
    m_HUD(resourceManager, config),
    m_levelNumber(0)
{
}

PauseState::~PauseState()
{
}

void PauseState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    const EnterPauseStateInformation* info = dynamic_cast<const EnterPauseStateInformation*>(enterInformation);
    m_level = info->m_level;
    m_levelNumber = info->m_levelNumber;

    State::onEnter(enterInformation, time);
    m_HUD.onEnter(m_level);
    m_HUD.restartAt(getPassedTime());

    m_playStateInfo.m_levelNumber = enterInformation->m_levelNumber;
    m_optionStateInfo.m_levelNumber = enterInformation->m_levelNumber;

    m_menu.updateLayout();

    m_menu.setCoinToolTipText(utility::replace(utility::translateKey("tooltip_coins"), 
                                           utility::toString(m_config.get<int>("coins"))));
}

StateChangeInformation PauseState::update(const float time)
{
    if(State::isPaused())
        return StateChangeInformation::Empty();

    updateTime(time);

    int clicked = -1;
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.update(m_screen, getPassedTime());

    if(utility::Keyboard.isKeyDown(sf::Keyboard::P)
        || utility::Keyboard.isKeyDown(sf::Keyboard::Pause)
        || PauseMenu::BUTTON_CONTINUE == clicked)
    {
        m_playStateInfo.m_returnFromPause = true;
        m_playStateInfo.m_level = m_level;
        m_transitionStateInfo.m_level = m_level;
        m_transitionStateInfo.m_followingState = PlayStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
        m_transitionStateInfo.m_comeFromeState = PauseStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == PauseMenu::BUTTON_OPTIONS)
    {
        m_optionStateInfo.m_comeFromState = PauseStateId;
        m_optionStateInfo.m_level = m_level;
        m_optionStateInfo.m_prepareOnly = false;
        m_transitionStateInfo.m_level = m_level;
        m_transitionStateInfo.m_followingState = OptionMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_optionStateInfo;
        m_transitionStateInfo.m_comeFromeState = PauseStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == PauseMenu::BUTTON_RESTART_LEVEL)
    {
        m_loadLevelStateInfo.m_prepareOnly = true;
        m_loadLevelStateInfo.m_directPlay = false;
        m_loadLevelStateInfo.m_level = m_level;
        m_loadLevelStateInfo.m_levelNumber = m_playStateInfo.m_levelNumber;
        m_transitionStateInfo.m_level = m_level;
        m_transitionStateInfo.m_followingState = LoadLevelStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_loadLevelStateInfo;
        m_transitionStateInfo.m_comeFromeState = PauseStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == PauseMenu::BUTTON_MAIN_MENU)
    {
        m_playStateInfo.m_prepareOnly = true;
        m_playStateInfo.m_returnFromPause = true;
        m_playStateInfo.m_level = m_level;
        m_transitionStateInfo.m_level = m_level;
        m_transitionStateInfo.m_followingState = MainMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
        m_transitionStateInfo.m_comeFromeState = PauseStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == PauseMenu::BUTTON_COINS)
    {
        m_coinShopStateInfo.m_level = m_level;
        m_coinShopStateInfo.m_prepareOnly = false;
        m_coinShopStateInfo.m_comeFromState = PauseStateId;
        m_coinShopStateInfo.m_levelNumber = m_levelNumber;
        m_transitionStateInfo.m_followingState = CoinShopStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_coinShopStateInfo;
        m_transitionStateInfo.m_comeFromeState = PauseStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    return StateChangeInformation::Empty();
}

void PauseState::draw(const DrawParameter& params)
{
    m_level->adjustView(params.getTarget());
    m_level->draw(params);
    m_HUD.update(m_level, getPassedTime());
    m_HUD.draw(params);

    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 128));
    params.getTarget().draw(whiteRect);

    m_menu.draw(params);
}
