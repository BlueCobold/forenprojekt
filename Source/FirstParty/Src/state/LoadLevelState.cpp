#include "LoadLevelState.hpp"

#include "../rendering/transitions/RandomTransition.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <memory>
#include <cstring>

LoadLevelState::LoadLevelState(sf::RenderWindow& screen,
                               ResourceManager& resourceManager,
                               AppConfig& config) :
    State(screen, resourceManager, config),
    m_label("gui_loading_screen",
            sf::Vector2f(ScreenLocation::Center, ScreenLocation::Center),
            sf::Vector2f(),
            0,
            resourceManager.getBitmapFont("red"),
            config.get<std::string>("language"),
            LineLabel::Left),
    m_lastLevel(nullptr),
    m_currentLevel(1),
    m_directPlay(false),
    m_transitionStateInfo(LoadLevelStateId)
{
    m_loadingErrorMessage = "";
    m_levelLoaderJob = std::unique_ptr<BackgroundLoader<LoadLevelState>>(new BackgroundLoader<LoadLevelState>(&LoadLevelState::loadLevel, *this));
    m_label.setOffset(sf::Vector2f(- m_label.getWidth() / 2.f, 0));
}

LoadLevelState::~LoadLevelState()
{
}

void LoadLevelState::onEnter(const EnterStateInformation* enterInformation, const double time)
{
    State::onEnter(enterInformation, time);

    const EnterLoadLevelStateInformation* info = dynamic_cast<const EnterLoadLevelStateInformation*>(enterInformation);
    m_directPlay = info->m_directPlay;
#ifdef LEVELTESTING
    m_file = info->m_file;
#endif
    if(enterInformation->m_prepareOnly)
        m_levelLoaderJob->reset();

    m_currentLevel = enterInformation->m_levelNumber;
}

std::unique_ptr<Level> LoadLevelState::gainLevel()
{
    return std::move(m_level);
}

StateChangeInformation LoadLevelState::update(const double time)
{
    std::string text(utility::translateKey(m_label.getLanguage() + "gui_loading_screen"));

    m_label.updateLayout(static_cast<sf::Vector2f>(m_screen.getSize()));

    updateTime(time);
    int step = static_cast<int>(getPassedTime() * 2) % 4;

    if(m_levelLoaderJob->isLoaded())
    {
        if(m_loadingErrorMessage.length() != 0)
            throw std::runtime_error(m_loadingErrorMessage);

        m_playStateInfo.m_returnFromPause = false;
        m_playStateInfo.m_level = m_lastLevel;
        m_playStateInfo.m_levelNumber = m_currentLevel;
        m_transitionStateInfo.m_level = m_lastLevel;

        if(m_directPlay)
            m_transitionStateInfo.m_followingState = PlayStateId;
        else
            m_transitionStateInfo.m_followingState = LevelPreviewStateId;

        m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
        m_levelLoaderJob->reset();
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }

    if(!m_levelLoaderJob->isLoading() && !m_levelLoaderJob->isLoaded())
    {
        m_levelLoaderJob->run();
    }


    for(int i = 0; i < step; ++i)
        text.append(".");

    m_label.setText(text);

    return StateChangeInformation::Empty();
}

void LoadLevelState::loadLevel()
{
    m_loadingErrorMessage = "";
    try
    {
#ifdef LEVELTESTING
        m_level = std::unique_ptr<Level>(new Level(m_file, m_currentLevel, getResourceManager(), m_config));
#else
        m_level = std::unique_ptr<Level>(new Level(m_currentLevel, getResourceManager(), m_config));
#endif
        m_lastLevel = m_level.get();
    }
    catch(std::bad_alloc& e)
    {
        m_loadingErrorMessage = e.what();
    }
    catch(std::runtime_error& e)
    {
        m_loadingErrorMessage = e.what();
    }
}

void LoadLevelState::doDraw(const DrawParameter& params)
{
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), params.getTarget().getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 255));
    params.getTarget().draw(whiteRect);

    m_label.draw(params);
}

void LoadLevelState::onEvent(utility::Event::EventType type)
{
    if(type == utility::Event::Resized)
        m_label.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_label.getWidth() / 2.f, m_screen.getSize().y / 2.f));
}

void LoadLevelState::setLanguage(const std::string& language)
{
    m_label.setLanguage(language);
    std::string text(utility::translateKey(m_label.getLanguage() + "gui_loading_screen"));
    m_label.setText(text);
}