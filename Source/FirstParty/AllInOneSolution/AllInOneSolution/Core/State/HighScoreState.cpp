#include "HighScoreState.hpp"
#include "../Input.hpp"
#include "../resources/Config.hpp"
#include "../resources/ResourceManager.hpp"
#include "../rendering/transitions/RandomTransition.hpp"
#include "../model/Level.hpp"
#include "../gui/CheckBox.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Network/Http.hpp>
#include <string>

HighScoreState::HighScoreState(sf::RenderWindow& screen, 
                               ResourceManager& resourceManager, 
                               AppConfig& config) :
    State(screen, resourceManager, config),
    m_menu(sf::Vector2f(0, 0), screen, resourceManager),
    m_HUD(resourceManager, config),
    m_onlineHighscore(false),
    m_loaded(false),
    loadingOnlineHighScoreThread(nullptr)
{
        loadingOnlineHighScoreThread = std::unique_ptr<sf::Thread>(new sf::Thread(&HighScoreState::loadOnlineHighscore, this));
}

HighScoreState::~HighScoreState()
{
}

void HighScoreState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    const EnterHighScoreStateInformation* info = dynamic_cast<const EnterHighScoreStateInformation*>(enterInformation);
    
    m_highScoreStateInfo.m_comeFromState = info->m_comeFromState;
    m_highScoreStateInfo.m_level = info->m_level;
    m_highScoreStateInfo.m_levelNumber = info->m_level->number();

    State::onEnter(info, time);
    loadHighScore();
    m_menu.updateLayout();

    m_onlineHighscore = false;
    m_menu.getCheckbox(HighScoreMenu::CHECKBOX_GLOBAL_HIGHSCORE).setChecked(false);
}

StateChangeInformation HighScoreState::update(const float time)
{
    if(State::isPaused())
        return StateChangeInformation::Empty();

    std::string text(utility::translateKey("gui_loading_screen"));

    updateTime(time - m_timeDiff);

    int clicked = -1;
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.update(m_screen, getPassedTime());

    if(clicked == HighScoreMenu::BUTTON_CLOSE)
    { 
        m_stateInfo.m_prepareOnly = true;
        m_stateInfo.m_level = m_highScoreStateInfo.m_level;
        m_stateInfo.m_levelNumber = m_highScoreStateInfo.m_levelNumber;
        m_transitionStateInfo.m_followingState = m_highScoreStateInfo.m_comeFromState;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;

        m_onlineHighscore = false;
        m_loadInProgress = false;
        m_loaded = false;
        loadingOnlineHighScoreThread->terminate();

        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }

    if(m_menu.getCheckbox(HighScoreMenu::CHECKBOX_GLOBAL_HIGHSCORE).getChecked() && !m_onlineHighscore)
    {
        m_onlineHighscore = true;
        m_loadInProgress = true;
        clearHighScore();
        loadingOnlineHighScoreThread->launch();
    }
    else if(!m_menu.getCheckbox(HighScoreMenu::CHECKBOX_GLOBAL_HIGHSCORE).getChecked() && m_onlineHighscore)
    {
        m_onlineHighscore = false;
        m_loadInProgress = false;
        m_loaded = false;
        loadingOnlineHighScoreThread->terminate();
        loadHighScore();
    }
    else if(m_onlineHighscore && m_loadInProgress && !m_loaded)
    {
        int step = static_cast<int>(getPassedTime() * 2) % 4;
        for (int i = 0; i < step; ++i)
            text.append(".");
        m_menu.getLabel(HighScoreMenu::LABEL_LOADING).setText(text);
    }
    if(m_loaded && !m_loadInProgress)
        m_menu.getLabel(HighScoreMenu::LABEL_LOADING).setText("");

    return StateChangeInformation::Empty();
}

void HighScoreState::draw(const DrawParameter& params)
{    
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    if(m_highScoreStateInfo.m_level != nullptr)
    {
        m_highScoreStateInfo.m_level->adjustView(params.getTarget());
        m_HUD.update(m_highScoreStateInfo.m_level, getPassedTime());
        m_highScoreStateInfo.m_level->draw(params);
        m_HUD.draw(params);
        whiteRect.setFillColor(sf::Color(255, 255, 255, 128));
    }
    else
        whiteRect.setFillColor(sf::Color(255, 255, 255, 255));
    params.getTarget().draw(whiteRect);

    m_menu.draw(params);
}

void HighScoreState::loadHighScore()
{
    std::string number = utility::toString(m_highScoreStateInfo.m_level->number());

    m_menu.getLabel(HighScoreMenu::LABEL_LOADING).setText("");

    std::string mode = "NAM";

    if(m_highScoreStateInfo.m_level->isTimeAttackMode())
        mode = "TAM";

    for(int i = 0; i < 5; ++i)
    {
        // read the place data from stash.dat
        m_menu.getLabel(HighScoreMenu::LABEL_PLACES + i).setText(State::m_config.get<std::string>("HighScoreLevel" + number + "_Name" + utility::toString(i + 1) + mode));
        // reade the point data from stash.dat
        m_menu.getLabel(HighScoreMenu::LABEL_POINTS + i).setText(State::m_config.get<std::string>("HighScoreLevel" + number + "_Points" + utility::toString(i + 1) + mode));
    }
}
void HighScoreState::loadOnlineHighscore()
{
    std::string number = utility::toString(m_highScoreStateInfo.m_level->number());

    sf::Http http;
    http.setHost(m_config.get<std::string>("HighscoreServer"));
    
    sf::Http::Request request(m_config.get<std::string>("HighscorePath") + number);
    sf::Http::Response response = http.sendRequest(request);

    if(response.getStatus() != sf::Http::Response::Ok)
    {
        m_menu.getLabel(HighScoreMenu::LABEL_LOADING).setText(utility::translateKey("gui_not_available"));
        m_loaded = true;
        return;
    }

    FileReader onlineString(response.getBody(), false);

    std::string mode = "NAM";

    if(m_highScoreStateInfo.m_level->isTimeAttackMode())
        mode = "TAM";

    for(int i = 0; i < 5; ++i)
    {
        // read the place data from online server
        m_menu.getLabel(HighScoreMenu::LABEL_PLACES + i).setText(onlineString.get("HighScoreLevel" + number + "_Name" + utility::toString(i + 1) + mode));
        // read the point data from online server
        m_menu.getLabel(HighScoreMenu::LABEL_POINTS + i).setText(onlineString.get("HighScoreLevel" + number + "_Points" + utility::toString(i + 1) + mode));
    }

    m_loaded = true;
    m_loadInProgress = false;
}

void HighScoreState::clearHighScore()
{
    for(int i = 0; i < 5; ++i)
    {
        m_menu.getLabel(HighScoreMenu::LABEL_PLACES + i).setText("");
        m_menu.getLabel(HighScoreMenu::LABEL_POINTS + i).setText("");
    }
}