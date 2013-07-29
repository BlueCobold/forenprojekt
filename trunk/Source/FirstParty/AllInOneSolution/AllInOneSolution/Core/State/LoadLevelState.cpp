#include "LoadLevelState.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "../rendering/transitions/RandomTransition.hpp"

#include <memory>
#include <cstring>

LoadLevelState::LoadLevelState(sf::RenderWindow& screen, 
                               ResourceManager& resourceManager,
                               AppConfig& config) :
    State(screen, resourceManager, config),
    m_label(utility::translateKey("gui_loading_screen"), sf::Vector2f(screen.getSize().x / 2.f, screen.getSize().y / 2.f), 0, resourceManager.getBitmapFont("red"), LineLabel::Left),
    m_level(nullptr),
    m_lastLevel(nullptr),
    loadingLevelThread(nullptr),    
    m_loaded(false),
    m_loadInProgress(false)
{
    m_loadingErrorMessage[0] = '\0';
    loadingLevelThread = std::unique_ptr<sf::Thread>(new sf::Thread(&LoadLevelState::loadLevel, this));
    m_label.setPosition(m_screen.getSize().x / 2.f - m_label.getWidth() / 2.f, m_screen.getSize().y / 2.f);
}

LoadLevelState::~LoadLevelState()
{
}

void LoadLevelState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    State::onEnter(enterInformation, time);
    m_label.setPosition(m_screen.getSize().x / 2.f - m_label.getWidth() / 2.f, m_screen.getSize().y / 2.f);
    if(!enterInformation->m_prepareOnly)
    {
        m_loaded = false;
        m_loadInProgress = false;
    }
    else
        m_loadInProgress = true;
}

std::unique_ptr<Level> LoadLevelState::gainLevel()
{
    return std::move(m_level);
}

StateChangeInformation LoadLevelState::update(const float time)
{
    std::string text(utility::translateKey("gui_loading_screen"));

    updateTime(time);
    int step = static_cast<int>(getPassedTime() * 2) % 4;

    if(m_loaded)
    {
        if(m_loadingErrorMessage[0] != '\0')
            throw std::runtime_error(m_loadingErrorMessage);

        m_playStateInfo.m_returnFromPause = false;
        m_playStateInfo.m_level = m_lastLevel;
        m_transitionStateInfo.m_level = m_lastLevel;
        //m_transitionStateInfo.m_followingState = PlayStateId;
        m_transitionStateInfo.m_followingState = LevelPreviewStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }

    if(!m_loadInProgress)
    {
        // set this variable first, because if it gets set in the thread, nobody can assure
        // that this update-method will not get entered again before the variable gets set
        m_loadInProgress = true;
        loadingLevelThread->launch();
    }
    
    for (int i = 0;i < step;++i)
        text.append(".");

    m_label.setText(text);

    return StateChangeInformation::Empty();
}

void LoadLevelState::loadLevel()
{
    m_loadingErrorMessage[0] = '\0';
    try
    {
        m_level = std::unique_ptr<Level>(new Level(7, m_resourceManager, m_config));
        m_lastLevel = m_level.get();
    }
    catch(std::runtime_error e)
    {
        strcpy_s(m_loadingErrorMessage, sizeof(m_loadingErrorMessage), e.what());
    }
    m_loaded = true;
}

void LoadLevelState::draw(const DrawParameter& params)
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
        m_label.setPosition(m_screen.getSize().x / 2.f - m_label.getWidth() / 2.f, m_screen.getSize().y / 2.f);
}
