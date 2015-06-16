#include "PlayState.hpp"
#include "../Input.hpp"
#include "../resources/Config.hpp"
#include "../resources/ResourceManager.hpp"
#include "../rendering/transitions/RandomTransition.hpp"
#ifdef LEVELTESTING
#include "../resources/OpenFileDialoge.hpp"
#endif
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Network/Http.hpp>

PlayState::PlayState(sf::RenderWindow& screen, 
                     ResourceManager& resourceManager, 
                     AppConfig& config) :
    State(screen, resourceManager, config),
    m_level(nullptr),
    m_hud(resourceManager, config),
    m_shouldPause(false),
    m_minPoints(0),
    m_onlineHighScoreLoaderJob(nullptr)
{
    m_onlineHighScoreLoaderJob = std::unique_ptr<BackgroundLoader<PlayState>>(new BackgroundLoader<PlayState>(&PlayState::loadOnlineHighscore, *this));
}

PlayState::~PlayState()
{

}

void PlayState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    m_shouldPause = false;
    State::onEnter(enterInformation, time);

    const EnterPlayStateInformation* info = dynamic_cast<const EnterPlayStateInformation*>(enterInformation);
    m_level = info->m_level;

    m_hud.onEnter(m_level);

    utility::Mouse.capture();
    utility::Mouse.hideSystemCursor(m_screen);
    m_level->adaptToMouse();

    if(!info->m_returnFromPause)
    {
        m_level->restartAt(getPassedTime());
        m_level->update(getPassedTime());
        m_hud.update(m_level, getPassedTime());
        m_hud.skipInterpolation();
    }

    m_level->onEnter();
    m_pauseStateInfo.m_levelNumber = enterInformation->m_levelNumber;

    if(!m_onlineHighScoreLoaderJob->isLoaded() && !m_onlineHighScoreLoaderJob->isLoading())
        m_onlineHighScoreLoaderJob->run();
}

StateChangeInformation PlayState::update(const float time)
{
    updateTime(time);

    if(!isPaused())
    {
        utility::Mouse.startInterpolation(dynamic_cast<sf::Window&>(m_screen));
        m_level->update(getPassedTime());
        m_hud.update(m_level, getPassedTime());
        if(utility::Keyboard.isKeyDown(sf::Keyboard::F5) || utility::Keyboard.isKeyPressed(sf::Keyboard::F5))
        {
#ifdef LEVELTESTING
            if(m_level->number() == -1)
            {
                m_loadLevelStateInfo.m_file = m_level->getFileName();
                m_loadLevelStateInfo.m_level = nullptr;
                m_loadLevelStateInfo.m_directPlay = true;
                m_loadLevelStateInfo.m_levelNumber = -1;
            }
            else
            {
                m_loadLevelStateInfo.m_file = "";
                m_loadLevelStateInfo.m_level = m_level;
                m_loadLevelStateInfo.m_directPlay = false;
                m_loadLevelStateInfo.m_levelNumber = m_pauseStateInfo.m_levelNumber;
            }
            m_loadLevelStateInfo.m_prepareOnly = false;
            m_transitionStateInfo.m_followingState = LoadLevelStateId;
            m_transitionStateInfo.m_onEnterInformation = &m_loadLevelStateInfo;
            m_transitionStateInfo.m_comeFromeState = PlayStateId;
            m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
            return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
#else
            m_loadLevelStateInfo.m_prepareOnly = false;
            m_loadLevelStateInfo.m_level = nullptr;
            m_loadLevelStateInfo.m_levelNumber = m_pauseStateInfo.m_levelNumber;
            m_loadLevelStateInfo.m_directPlay = true;
            m_transitionStateInfo.m_followingState = LoadLevelStateId;
            m_transitionStateInfo.m_onEnterInformation = &m_loadLevelStateInfo;
            m_transitionStateInfo.m_comeFromeState = PlayStateId;
            m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
            m_onlineHighScoreLoaderJob->reset();
            return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
#endif
        }
        /* not needed anymore, since we got a button for it
        if(utility::Keyboard.isKeyDown(sf::Keyboard::T))
        {
            m_level->setTimeAttackMode(true);
        }*/
#ifdef LEVELTESTING
        if(utility::Keyboard.isKeyDown(sf::Keyboard::L) || utility::Keyboard.isKeyPressed(sf::Keyboard::L))
        {
            OpenFileDialoge ofd("Level\0*.lvl\0");
            bool result = ofd.openDialoge();
            utility::Keyboard.notifyKeyReleased(sf::Keyboard::L);
            if(result)        
                m_loadLevelStateInfo.m_file = ofd.getFile();
            else
                return StateChangeInformation::Empty();
            m_loadLevelStateInfo.m_level = nullptr;
            m_loadLevelStateInfo.m_prepareOnly = false;
            m_loadLevelStateInfo.m_levelNumber = -1;
            m_loadLevelStateInfo.m_directPlay = true;
            m_transitionStateInfo.m_followingState = LoadLevelStateId;
            m_transitionStateInfo.m_onEnterInformation = &m_loadLevelStateInfo;
            m_transitionStateInfo.m_comeFromeState = PlayStateId;
            m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
            return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
        }
#endif
        if(m_shouldPause || utility::Keyboard.isKeyDown(sf::Keyboard::P) || 
           utility::Keyboard.isKeyDown(sf::Keyboard::Pause) || utility::Keyboard.isKeyDown(sf::Keyboard::Escape))
        {
            m_pauseStateInfo.m_level = m_level;
            m_transitionStateInfo.m_level = m_level;
            m_transitionStateInfo.m_followingState = PauseStateId;
            m_transitionStateInfo.m_onEnterInformation = &m_pauseStateInfo;
            m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
            return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
        }
        
        if(m_level->isLevelPassed())
        {
            if(!checkForNewHighscore() && m_minPoints > m_level->getPoints())
            {
                m_pauseStateInfo.m_level = m_level;
                m_transitionStateInfo.m_onEnterInformation = &m_pauseStateInfo;
                m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
                m_transitionStateInfo.m_followingState = LevelPassStateId;
            }
            else
            {
                m_pauseStateInfo.m_level = m_level;
                m_transitionStateInfo.m_onEnterInformation = &m_pauseStateInfo;
                m_transitionStateInfo.m_followingState = NewHighScoreStateId;
                m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
            }

            return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
        }

        if(m_level->isLevelFailed())
        {
            m_pauseStateInfo.m_level = m_level;
            m_transitionStateInfo.m_followingState = LevelFailStateId;
            m_transitionStateInfo.m_onEnterInformation = &m_pauseStateInfo;
            m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
            m_onlineHighScoreLoaderJob->reset();
            return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
        }
    }
    else
    {
        m_pauseStateInfo.m_level = m_level;
        m_transitionStateInfo.m_level = m_level;
        m_transitionStateInfo.m_followingState = PauseStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_pauseStateInfo;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }

    return StateChangeInformation::Empty();
}

void PlayState::draw(const DrawParameter& params)
{
    m_level->adjustView(params);
    m_level->draw(params);
    m_hud.draw(params);
}

void PlayState::onEvent(utility::Event::EventType type)
{
    if(type == utility::Event::LostFocus)
        m_shouldPause = true;
}

bool PlayState::checkForNewHighscore()
{
    int points = m_level->getPoints();
    float time = m_level->getLevelPlayTime();
    unsigned int number = m_level->number();
    if(m_level->isTimeAttackMode())
    {
        for(int i = 5; i > 0; --i)
        {
            std::string term = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(i) + "TAM";
            if(points > State::m_config.get<int>(term))
                return true;
        }
    }
    else
    {
        for(int i = 5; i > 0; --i)
        {
            std::string termPoints = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(i) + "NAMP";
            std::string termTime = "HighScoreLevel" + utility::toString(number) + "_Points" + utility::toString(i) + "NAMT";
            if(points > State::m_config.get<int>(termPoints) || time < State::m_config.get<float>(termTime))
                return true;
        }
    }
    return false;
}

void PlayState::loadOnlineHighscore()
{
    std::string number = utility::toString(m_level->number());

    sf::Http http;
    http.setHost(m_config.get<std::string>("HighscoreServer"));
    
    sf::Http::Request request(m_config.get<std::string>("HighscorePath") + "highscore.php?lvl=" + number);
    sf::Http::Response response = http.sendRequest(request);

    if(response.getStatus() != sf::Http::Response::Ok)
        return;

    FileReader onlineString(response.getBody(), false);

    std::string mode = "NAM";

    if(m_level->isTimeAttackMode())
        mode = "TAM";

    m_minPoints = utility::stringTo<int>(onlineString.get("HighScoreLevel" + number + "_Points" + utility::toString(5) + mode));
}