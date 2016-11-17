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
#include "../gui/SubWindow.hpp"

MainMenuState::MainMenuState(sf::RenderWindow& screen,
                             ResourceManager& resourceManager,
                             AppConfig& config) :
    State(screen, resourceManager, config),
    m_menu(screen, resourceManager),
    m_transitionStateInfo(MainMenuStateId),
    m_optionStateInfo(MainMenuStateId)
{
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::onEnter(const EnterStateInformation* enterInformation, const double time)
{
    State::onEnter(enterInformation, time);

    auto language = m_config.get<std::string>("language");

    if(language == "en")
        m_menu.showEnglishLanguage();
    else if(language == "de")
        m_menu.showGermanLanguage();

    m_menu.showLanguageSelect(false);
    m_menu.update(m_screen, getPassedTime());
}

StateChangeInformation MainMenuState::update(const double time)
{
    updateTime(time);

    if(State::isPaused())
    {
        m_menu.update(m_screen, getPassedTime(), MenuElementType::Animation);
        return StateChangeInformation::Empty();
    }

    int clicked = -1;
    int subWindowClicked = -1;

    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.getSubWindow(MainMenu::SUBWINDOW_SELECT_LANGUAGE).getPanel()->registerOnClick([&](const Button& sender){ subWindowClicked = sender.getId(); });
    
    m_menu.update(m_screen, getPassedTime());

    if(clicked == MainMenu::BUTTON_START_NEW_GAME)
    {
        m_loadLevelStateInfo = EnterLoadLevelStateInformation();
        m_transitionStateInfo.m_followingState = LoadLevelStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_loadLevelStateInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == MainMenu::BUTTON_OPTIONS)
    {
        m_optionStateInfo = EnterOptionStateInformation(MainMenuStateId);
        m_transitionStateInfo.m_followingState = OptionMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_optionStateInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == MainMenu::BUTTON_CREDITS)
    {
        m_stateInfo = EnterStateInformation();
        m_transitionStateInfo.m_followingState = CreditMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == MainMenu::BUTTON_SELECT_LEVEL)
    {
#ifdef LEVELTESTING
        if(utility::Keyboard.isKeyDown(sf::Keyboard::L) || utility::Keyboard.isKeyPressed(sf::Keyboard::L))
        {
            m_loadLevelStateInfo = EnterLoadLevelStateInformation();
            OpenFileDialoge ofd("Level\0*.lvl\0");
            bool result = ofd.openDialoge();
            utility::Keyboard.notifyKeyReleased(sf::Keyboard::L);
            if(result)
                m_loadLevelStateInfo.m_file = ofd.getFile();
            else
                return StateChangeInformation::Empty();
            m_loadLevelStateInfo.m_levelNumber = 0;
            m_loadLevelStateInfo.m_directPlay = true;
            m_transitionStateInfo.m_followingState = LoadLevelStateId;
            m_transitionStateInfo.m_onEnterInformation = &m_loadLevelStateInfo;
            return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
        }
        else
        {
            m_stateInfo = EnterStateInformation();
            m_transitionStateInfo.m_followingState = LevelSelectStateId;
            m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;
            return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
        }
#else
        m_stateInfo = EnterStateInformation();
        m_transitionStateInfo.m_followingState = LevelSelectStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
#endif
    }
    else if(clicked == MainMenu::BUTTON_CLOSE)
        State::m_screen.close();
    else if(clicked == MainMenu::BUTTON_ACHIEVEMENT)
    {
        m_optionStateInfo = EnterOptionStateInformation(MainMenuStateId);
        m_transitionStateInfo.m_followingState = AchievementStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_optionStateInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == MainMenu::BUTTON_GERMAN_SHOW || clicked == MainMenu::BUTTON_ENGLISH_SHOW)
        m_menu.showLanguageSelect(true);
    else if(subWindowClicked == MainMenu::BUTTON_GERMAN_SELECT)
    {
        m_menu.showGermanLanguage();
        m_menu.showLanguageSelect(false);
        m_config.set("language", "de");
        utility::setLanguage("de");
    }
    else if(subWindowClicked == MainMenu::BUTTON_ENGLISH_SELECT)
    {
        m_menu.showEnglishLanguage();
        m_menu.showLanguageSelect(false);
        m_config.set("language", "en");
        utility::setLanguage("en");
    }
    return StateChangeInformation::Empty();
}

void MainMenuState::doDraw(const DrawParameter& params)
{
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 255));
    params.getTarget().draw(whiteRect);

    m_menu.draw(params);
}
