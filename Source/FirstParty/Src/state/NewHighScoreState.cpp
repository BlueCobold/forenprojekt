#include "NewHighScoreState.hpp"
#include "../Input.hpp"
#include "../model/Level.hpp"
#include "../resources/AppConfig.hpp"
#include "../resources/ResourceManager.hpp"
#include "../rendering/transitions/RandomTransition.hpp"
#include "../gui/InputBox.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Network/Http.hpp>

NewHighScoreState::NewHighScoreState(sf::RenderWindow& screen, 
                               ResourceManager& resourceManager, 
                               AppConfig& config) :
    State(screen, resourceManager, config),
    m_background(nullptr),
    m_menu(screen, resourceManager),
    m_HUD(resourceManager, config),
    m_replay(false),
    m_lastName(""),
    m_transitionStateInfo(NewHighScoreStateId)
{
}

NewHighScoreState::~NewHighScoreState()
{
}

void NewHighScoreState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    m_replay = false;
    const EnterPauseStateInformation* info = dynamic_cast<const EnterPauseStateInformation*>(enterInformation);
    m_level = info->m_level;

    State::onEnter(enterInformation, time);
    m_HUD.restartAt(getPassedTime());
    m_HUD.onEnter(m_level);

    m_menu.updateLayout();

    m_stateInfo.m_levelNumber = enterInformation->m_levelNumber;

    if(m_lastName == "")
    {
        std::string name = utility::getUserName();
        unsigned int maxLength = m_menu.getInputBox(NewHighScoreMenu::INPUTBOX).getInputLimit();
        if(name.size() > maxLength)
            m_menu.getInputBox(NewHighScoreMenu::INPUTBOX).setText(name.substr(0, maxLength - 1));
        else
            m_menu.getInputBox(NewHighScoreMenu::INPUTBOX).setText(name);
    }
    else
        m_menu.getInputBox(NewHighScoreMenu::INPUTBOX).setText(m_lastName);

#ifdef IOS
    m_menu.showKeyboard();
#endif
}

StateChangeInformation NewHighScoreState::update(const float time)
{
    if(State::isPaused())
        return StateChangeInformation::Empty();

    updateTime(time);

    m_HUD.update(m_level, getPassedTime());

    int clicked = -1;
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });

    m_menu.update(m_screen, getPassedTime());

    if(m_menu.getInputBox(NewHighScoreMenu::INPUTBOX).isFinished() || clicked == NewHighScoreMenu::BUTTON_OK)
    {
        m_lastName = m_menu.getInputBox(NewHighScoreMenu::INPUTBOX).getText();
        addNewHighScore(m_level->getPoints(), m_level->getLevelPlayTime(), m_lastName);

        m_stateInfo.m_levelNumber = m_level->number();
        m_stateInfo.m_level = m_level;
        m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;
        m_transitionStateInfo.m_followingState = LevelPassStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::Alpha;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    if(m_menu.getInputBox(NewHighScoreMenu::INPUTBOX).isActivatedByMouse())
        m_menu.showKeyboard();
#ifdef IOS
    if(!m_menu.isKeyboardShown())
        m_menu.getInputBox(NewHighScoreMenu::INPUTBOX).disableCaret();
#endif
    if(clicked == NewHighScoreMenu::BUTTON_SKIP)
    {
        m_stateInfo.m_levelNumber = m_level->number();
        m_stateInfo.m_level = m_level;
        m_stateInfo.m_prepareOnly = false;
        m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;
        m_transitionStateInfo.m_followingState = LevelPassStateId;
        m_transitionStateInfo.m_comeFromeState = NewHighScoreStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::Alpha;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }

    return StateChangeInformation::Empty();
}

void NewHighScoreState::draw(const DrawParameter& params)
{
    m_level->adjustView(params);
    m_level->draw(params);

    m_HUD.draw(params);
    
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 128));
    params.getTarget().draw(whiteRect);

    m_menu.draw(params);
}

void NewHighScoreState::addNewHighScore(int points, float time, std::string name)
{
    if(name == "")
    {
        m_menu.getInputBox(NewHighScoreMenu::INPUTBOX).setText(m_lastName);
        return;
    }

    int newPlacePoints = 0;
    int newPlaceTime = 0;
    int number = m_level->number();
    
    std::string mode = "NAM";

    if(m_level->isTimeAttackMode())
        mode = "TAM";

    sendDataToHighScoreServer(name);

    if(mode == "NAM")
    {
        for(int i = 5; i > 0; --i)
        {
            std::string term = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(i) + mode + "P";
            if(points > State::m_config.get<int>(term))
                newPlacePoints = i;
        }

        for(int i = 5; i > 0; --i)
        {
            std::string term = "HighScoreLevel" + utility::toString(number) + "_Time" + utility::toString(i) + mode + "T";
            if(time < State::m_config.get<float>(term))
                newPlaceTime = i;
        }
    }
    else if(mode == "TAM")
    {
        for(int i = 5; i > 0; --i)
        {
            std::string term = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(i) + mode;
            if(points > State::m_config.get<int>(term))
                newPlacePoints = i;
        }
    }
    else
        return;

    if(mode == "NAM" && (newPlacePoints > 0 || newPlaceTime > 0))
    {
        if(newPlacePoints > 0)
        {
            for(int i = 5; i > newPlacePoints; --i)
            {
                std::string termPoints1 = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(i - 1) + mode + "P";
                std::string termName1 = "HighScoreLevel" + utility::toString(number) + "_Name" + utility::toString(i - 1) + mode + "P";

                std::string termPoints2 = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(i) + mode + "P";
                std::string termName2 = "HighScoreLevel" + utility::toString(number) + "_Name" + utility::toString(i) + mode + "P";

                State::m_config.set<int>(termPoints2, State::m_config.get<int>(termPoints1));
                State::m_config.set<std::string>(termName2, State::m_config.get<std::string>(termName1));
            }

            std::string termPoints = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(newPlacePoints) + mode + "P";
            std::string termName = "HighScoreLevel" + utility::toString(number) + "_Name" + utility::toString(newPlacePoints) + mode + "P";

            State::m_config.set<int>(termPoints, points);
            State::m_config.set<std::string>(termName, name);
        }

        if(newPlaceTime > 0)
        {
            for(int i = 5; i > newPlaceTime; --i)
            {
                std::string termTime1 = "HighScoreLevel" + utility::toString(number) + "_Time" + utility::toString(i - 1) + mode + "T";
                std::string termName1 = "HighScoreLevel" + utility::toString(number) + "_Name" + utility::toString(i - 1) + mode + "T";

                std::string termTime2 = "HighScoreLevel" + utility::toString(number) + "_Time" + utility::toString(i) + mode + "T";
                std::string termName2 = "HighScoreLevel" + utility::toString(number) + "_Name" + utility::toString(i) + mode + "T";

                State::m_config.set<float>(termTime2, State::m_config.get<float>(termTime1));
                State::m_config.set<std::string>(termName2, State::m_config.get<std::string>(termName1));
            }

            std::string termTime = "HighScoreLevel" + utility::toString(number) + "_Time" + utility::toString(newPlaceTime) + mode + "T";
            std::string termName = "HighScoreLevel" + utility::toString(number) + "_Name" + utility::toString(newPlaceTime) + mode + "T";

            State::m_config.set<float>(termTime, time);
            State::m_config.set<std::string>(termName, name);
        }
    }
    else if(mode == "TAM")
    {
        for(int i = 5; i > newPlacePoints; --i)
        {
            std::string termPoints1 = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(i - 1) + mode;
            std::string termName1 = "HighScoreLevel" + utility::toString(number) + "_Name" + utility::toString(i - 1) + mode;

            std::string termPoints2 = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(i) + mode;
            std::string termName2 = "HighScoreLevel" + utility::toString(number) + "_Name" + utility::toString(i) + mode;

            State::m_config.set<int>(termPoints2, State::m_config.get<int>(termPoints1));
            State::m_config.set<std::string>(termName2, State::m_config.get<std::string>(termName1));
        }

        std::string termPoints = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(newPlacePoints) + mode;
        std::string termName = "HighScoreLevel" + utility::toString(number) + "_Name" + utility::toString(newPlacePoints) + mode;

        State::m_config.set<int>(termPoints, points);
        State::m_config.set<std::string>(termName, name);
    }
}

void NewHighScoreState::sendDataToHighScoreServer(const std::string& name) const
{
    std::string dataString = createHighscoreString(name);

    sf::Http http;
    http.setHost(m_config.get<std::string>("HighscoreServer"));

    sf::Http::Request request(m_config.get<std::string>("HighscorePath") + "add_highscore.php?" + dataString);

    sf::Http::Response response = http.sendRequest(request);
}

const std::string NewHighScoreState::createHighscoreString(const std::string& name) const
{
    std::string result;
    std::string value;

    unsigned int counterTeeter = 0;
    unsigned int counterTarget = 0;
    unsigned int counterBonusTarget = 0;
    unsigned int counterLostBall = 0;
    unsigned int counterGravity = 0;
    unsigned int counterTargetSpeeding = 0;
    unsigned int counterBonusTargetSpeeding = 0;
    unsigned int counterOtherGoody = 0;

    auto list = m_level->getGameEvents();

    result = "v=" + utility::VERSION + "&name=" + name;
    result.append("&lvl=" + utility::toString(m_level->number()) + "&points=" + utility::toString(m_level->getPoints()));

    if(m_level->isTimeAttackMode())
        result.append("&mode=TAM");
    else
        result.append("&mode=NAM");

    for(auto it = std::begin(list); it != std::end(list); ++it)
    {
        // get time as millisecond
        value = utility::toString(static_cast<unsigned int>(it->getPassedTime() * 1000));

        if(it->getEventType() == GameEvent::HitTeeter)
            result.append("&t[" + utility::toString(counterTeeter++) + "]=" + value);
        else if(it->getEventType() == GameEvent::HitTarget)
            result.append("&m[" + utility::toString(counterTarget++) + "]=" + value);
        else if(it->getEventType() == GameEvent::HitBonusTarget)
            result.append("&b[" + utility::toString(counterBonusTarget++) + "]=" + value);
        else if(it->getEventType() == GameEvent::LostBall)
            result.append("&l[" + utility::toString(counterLostBall++) + "]=" + value);
        else if(it->getEventType() == GameEvent::GravityGoody)
            result.append("&g[" + utility::toString(counterGravity++) + "]=" + value);
        else if(it->getEventType() == GameEvent::HitTargetSpeeding)
            result.append("&ms[" + utility::toString(counterTargetSpeeding++) + "]=" + value);
        else if(it->getEventType() == GameEvent::HitBonusTargetSpeeding)
            result.append("&bs[" + utility::toString(counterBonusTargetSpeeding++) + "]=" + value);
        else if(it->getEventType() == GameEvent::ExtraBallGoody ||
                it->getEventType() == GameEvent::ExtraTimeGoody ||
                it->getEventType() == GameEvent::InvulnerableGoody)
            result.append("&og[" + utility::toString(counterOtherGoody++) + "]=" + value);
        else
            throw std::runtime_error(utility::replace(utility::translateKey("UnknownGameEvent"), utility::toString(it->getEventType())));
    }

    return result;
}