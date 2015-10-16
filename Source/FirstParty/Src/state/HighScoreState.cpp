#include "HighScoreState.hpp"
#include "../Input.hpp"
#include "../resources/Config.hpp"
#include "../resources/ResourceManager.hpp"
#include "../rendering/transitions/RandomTransition.hpp"
#include "../model/Level.hpp"
#include "../gui/CheckBox.hpp"
#include "../gui/SubWindow.hpp"
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
    m_menu(screen, resourceManager),
    m_HUD(resourceManager, config),
    m_showPoints(true),
    m_offsetAdded(false),
    m_offset(0, 60),
    m_clicked(-1),
    m_transitionStateInfo(HighScoreStateId),
    m_onlineHighscore(false)
{
    m_onlineHighscoreLoaderJob = std::unique_ptr<BackgroundLoader<HighScoreState>>(new BackgroundLoader<HighScoreState>(&HighScoreState::loadOnlineHighscore, *this));

    auto buttonFunc = [&](const Button& sender){ m_clicked = sender.getId(); };
    m_menu.getButton(HighScoreMenu::BUTTON_TAB_TIME).registerOnPressed(buttonFunc);
    m_menu.getButton(HighScoreMenu::BUTTON_TAB_POINTS).registerOnPressed(buttonFunc);
    m_menu.getButton(HighScoreMenu::BUTTON_CLOSE).registerOnPressed(buttonFunc);
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

    if(m_highScoreStateInfo.m_level->isTimeAttackMode())
        m_showPoints = true;

    bool showButton = !m_highScoreStateInfo.m_level->isTimeAttackMode();
    m_menu.getButton(HighScoreMenu::BUTTON_TAB_TIME).setVisible(showButton);
    m_menu.getButton(HighScoreMenu::BUTTON_TAB_POINTS).setVisible(showButton);

    buildSubWindowElements();

    State::onEnter(info, time);
    loadHighScore();

    m_HUD.onEnter(m_highScoreStateInfo.m_level);

    m_onlineHighscore = false;
    m_menu.getCheckbox(HighScoreMenu::CHECKBOX_GLOBAL_HIGHSCORE).setChecked(m_onlineHighscore);
    m_menu.getCheckbox(HighScoreMenu::CHECKBOX_LOCAL_HIGHSCORE).setChecked(!m_onlineHighscore);
}

StateChangeInformation HighScoreState::update(const float time)
{
    if(State::isPaused())
        return StateChangeInformation::Empty();

    std::string text(utility::translateKey("gui_loading_screen"));

    m_clicked = -1;

    updateTime(time - m_timeDiff);
    m_menu.update(m_screen, getPassedTime());

    if(m_menu.getCheckbox(HighScoreMenu::CHECKBOX_LOCAL_HIGHSCORE).getChecked())
    {
        if(m_clicked == HighScoreMenu::BUTTON_TAB_TIME)
        {
            m_showPoints = false;
            loadHighScore();
        }

        if(m_clicked == HighScoreMenu::BUTTON_TAB_POINTS)
        {
            m_showPoints = true;
            loadHighScore();
        }
    }
    else if(m_menu.getCheckbox(HighScoreMenu::CHECKBOX_GLOBAL_HIGHSCORE).getChecked())
    {
        if(m_clicked == HighScoreMenu::BUTTON_TAB_TIME)
        {
            m_showPoints = false;
            clearHighScore();
            m_onlineHighscoreLoaderJob->reset();
            m_onlineHighscoreLoaderJob->run();
        }

        if(m_clicked == HighScoreMenu::BUTTON_TAB_POINTS)
        {
            m_showPoints = true;
            clearHighScore();
            m_onlineHighscoreLoaderJob->reset();
            m_onlineHighscoreLoaderJob->run();
        }
    }

    if(m_clicked == HighScoreMenu::BUTTON_CLOSE)
    {
        m_stateInfo.m_prepareOnly = true;
        m_stateInfo.m_level = m_highScoreStateInfo.m_level;
        m_stateInfo.m_levelNumber = m_highScoreStateInfo.m_levelNumber;
        m_transitionStateInfo.m_followingState = m_highScoreStateInfo.m_comeFromState;
        m_onlineHighscoreLoaderJob->reset();
        m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;

        m_onlineHighscore = false;
        m_onlineHighscoreLoaderJob->stop();

        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }

    if(m_menu.getCheckbox(HighScoreMenu::CHECKBOX_GLOBAL_HIGHSCORE).getChecked() && !m_onlineHighscore)
    {
        m_menu.getCheckbox(HighScoreMenu::CHECKBOX_LOCAL_HIGHSCORE).setChecked(false);
        m_onlineHighscore = true;
        clearHighScore();
        m_onlineHighscoreLoaderJob->run();
    }
    else if(m_menu.getCheckbox(HighScoreMenu::CHECKBOX_LOCAL_HIGHSCORE).getChecked() && m_onlineHighscore)
    {
        m_menu.getCheckbox(HighScoreMenu::CHECKBOX_GLOBAL_HIGHSCORE).setChecked(false);
        m_onlineHighscore = false;
        m_onlineHighscoreLoaderJob->reset();
        loadHighScore();
    }
    else if(m_onlineHighscore && m_onlineHighscoreLoaderJob->isLoading() && !m_onlineHighscoreLoaderJob->isLoaded())
    {
        int step = static_cast<int>(getPassedTime() * 2) % 4;
        for(int i = 0; i < step; ++i)
            text.append(".");
        m_menu.getLabel(HighScoreMenu::LABEL_LOADING).setText(text);
    }
    if(m_onlineHighscoreLoaderJob->isLoaded() && !m_onlineHighscoreLoaderJob->isLoading())
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
        m_highScoreStateInfo.m_level->adjustView(params);
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

    std::string mode = "NAMP";

    if(!m_showPoints)
        mode = "NAMT";

    if(m_highScoreStateInfo.m_level->isTimeAttackMode())
        mode = "TAM";

    for(int i = 0; i < 5; ++i)
    {
        std::string text = "";
        // read the place data from stash.dat
        m_menu.getLabel(HighScoreMenu::LABEL_PLACES + i).setText(State::m_config.get<std::string>("HighScoreLevel" + number + "_Name" + utility::toString(i + 1) + mode));
        // reade the point data from stash.dat
        if(m_showPoints)
            text = State::m_config.get<std::string>("HighScoreLevel" + number + "_Points" + utility::toString(i + 1) + mode);
        else
            text = utility::floatToPlayTimeString(State::m_config.get<float>("HighScoreLevel" + number + "_Time" + utility::toString(i + 1) + mode));
        m_menu.getLabel(HighScoreMenu::LABEL_POINTS + i).setText(text);
    }
}
void HighScoreState::loadOnlineHighscore()
{
    std::string number = utility::toString(m_highScoreStateInfo.m_level->number());

    sf::Http http;
    http.setHost(m_config.get<std::string>("HighscoreServer"));

    sf::Http::Request request(m_config.get<std::string>("HighscorePath") + "highscore.php?lvl=" + number);
    sf::Http::Response response = http.sendRequest(request);

    if(response.getStatus() != sf::Http::Response::Ok)
    {
        m_menu.getLabel(HighScoreMenu::LABEL_LOADING).setText(utility::translateKey("gui_not_available"));
        return;
    }

    FileReader onlineString(response.getBody(), false);

    std::string mode = "NAMP";

    if(!m_showPoints)
        mode = "NAMT";

    if(m_highScoreStateInfo.m_level->isTimeAttackMode())
        mode = "TAM";

    for(int i = 0; i < 5; ++i)
    {
        std::string text = "";
        // read the place data from online server
        m_menu.getLabel(HighScoreMenu::LABEL_PLACES + i).setText(onlineString.get("HighScoreLevel" + number + "_Name" + utility::toString(i + 1) + mode));
        // read the point data from online server
        if(m_showPoints)
            text = onlineString.get("HighScoreLevel" + number + "_Points" + utility::toString(i + 1) + mode);
        else
            text = utility::floatToPlayTimeString(utility::stringTo<float>(onlineString.get("HighScoreLevel" + number + "_Time" + utility::toString(i + 1) + mode)));

        m_menu.getLabel(HighScoreMenu::LABEL_POINTS + i).setText(text);
    }
}

void HighScoreState::clearHighScore()
{
    for(int i = 0; i < 5; ++i)
    {
        m_menu.getLabel(HighScoreMenu::LABEL_PLACES + i).setText("");
        m_menu.getLabel(HighScoreMenu::LABEL_POINTS + i).setText("");
    }
}

void HighScoreState::buildSubWindowElements()
{
    if(!m_highScoreStateInfo.m_level->isTimeAttackMode() && !m_offsetAdded)
    {
        for(int i = 0; i < 5; ++i)
        {
            m_menu.getLabel(HighScoreMenu::LABEL_PLACES + i).setPosition(m_menu.getLabel(HighScoreMenu::LABEL_PLACES + i).getPosition() + m_offset);
            m_menu.getLabel(HighScoreMenu::LABEL_POINTS + i).setPosition(m_menu.getLabel(HighScoreMenu::LABEL_POINTS + i).getPosition() + m_offset);
        }
        m_menu.getSubWindow(HighScoreMenu::SUBWINDOW).setInnerHeight(m_menu.getSubWindow(HighScoreMenu::SUBWINDOW).getInnerHeight() + static_cast<int>(m_offset.y));
        m_offsetAdded = true;
    }
    else if(m_highScoreStateInfo.m_level->isTimeAttackMode() && m_offsetAdded)
    {
        for(int i = 0; i < 5; ++i)
        {
            m_menu.getLabel(HighScoreMenu::LABEL_PLACES + i).setPosition(m_menu.getLabel(HighScoreMenu::LABEL_PLACES + i).getPosition() - m_offset);
            m_menu.getLabel(HighScoreMenu::LABEL_POINTS + i).setPosition(m_menu.getLabel(HighScoreMenu::LABEL_POINTS + i).getPosition() - m_offset);
        }
        m_menu.getSubWindow(HighScoreMenu::SUBWINDOW).setInnerHeight(m_menu.getSubWindow(HighScoreMenu::SUBWINDOW).getInnerHeight() - static_cast<int>(m_offset.y));
        m_offsetAdded = false;
    }
}
