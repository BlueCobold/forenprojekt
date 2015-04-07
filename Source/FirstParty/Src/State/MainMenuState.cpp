#include "MainMenuState.hpp"
#include "../Input.hpp"
#include "../resources/Config.hpp"
#include "../resources/ResourceManager.hpp"
#include "../rendering/transitions/RandomTransition.hpp"
#ifdef LEVELTESTING
#include "../resources/OpenFileDialoge.hpp"
#endif

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

MainMenuState::MainMenuState(sf::RenderWindow& screen, 
                             ResourceManager& resourceManager, 
                             AppConfig& config) :
    State(screen, resourceManager, config),
    m_menu(screen, resourceManager)
{
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    State::onEnter(enterInformation, time);
    m_menu.updateLayout();
}

StateChangeInformation MainMenuState::update(const float time)
{
    updateTime(time);

    if(State::isPaused())
    {
        m_menu.update(m_screen, getPassedTime(), MenuElementType::Animation);
        return StateChangeInformation::Empty();
    }

    int clicked = -1;
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.update(m_screen, getPassedTime());

    if(clicked == MainMenu::BUTTON_START_NEW_GAME)
    {
        m_loadLevelStateInfo.m_prepareOnly = false;
        m_loadLevelStateInfo.m_level = nullptr;
        m_loadLevelStateInfo.m_levelNumber = 1;
        m_loadLevelStateInfo.m_directPlay = false;
        m_transitionStateInfo.m_followingState = LoadLevelStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_loadLevelStateInfo;
        m_transitionStateInfo.m_comeFromeState = MainMenuStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == MainMenu::BUTTON_OPTIONS)
    {
        m_optionStateInfo.m_comeFromState = MainMenuStateId;
        m_optionStateInfo.m_level = nullptr;
        m_optionStateInfo.m_prepareOnly = false;
        m_transitionStateInfo.m_followingState = OptionMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_optionStateInfo;
        m_transitionStateInfo.m_comeFromeState = MainMenuStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == MainMenu::BUTTON_CREDITS)
    {
        m_stateInfo.m_prepareOnly = false;
        m_stateInfo.m_level = nullptr;
        m_transitionStateInfo.m_followingState = CreditMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;
        m_transitionStateInfo.m_comeFromeState = MainMenuStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == MainMenu::BUTTON_SELECT_LEVEL)
    {
#ifdef LEVELTESTING
        OpenFileDialoge ofd("Level\0*.lvl\0");
        if(ofd.openDialoge())        
            m_loadLevelStateInfo.m_file = ofd.getFile();
        else
            return StateChangeInformation::Empty();
        m_loadLevelStateInfo.m_level = nullptr;
        m_loadLevelStateInfo.m_prepareOnly = false;
        m_loadLevelStateInfo.m_levelNumber = -1;
        m_loadLevelStateInfo.m_directPlay = true;
        m_transitionStateInfo.m_followingState = LoadLevelStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_loadLevelStateInfo;
        m_transitionStateInfo.m_comeFromeState = MainMenuStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
#else
        m_stateInfo.m_prepareOnly = false;
        m_stateInfo.m_level = nullptr;
        m_transitionStateInfo.m_followingState = LevelSelectStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;
        m_transitionStateInfo.m_comeFromeState = MainMenuStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
#endif
    }
    else if(clicked == MainMenu::BUTTON_CLOSE)
        State::m_screen.close();

    return StateChangeInformation::Empty();
}

void MainMenuState::draw(const DrawParameter& params)
{    
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 255));
    params.getTarget().draw(whiteRect);

    m_menu.draw(params);
}