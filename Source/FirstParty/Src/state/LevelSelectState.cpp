
#include "LevelSelectState.hpp"

#include "../gui/Button.hpp"
#include "../model/Level.hpp"
#include "../rendering/transitions/RandomTransition.hpp"
#include "../resources/Config.hpp"
#include "../resources/SpriteSheet.hpp"
#include "../MacHelper.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

#include <tinyxml2.h>

LevelSelectState::LevelSelectState(sf::RenderWindow& screen, 
                                 ResourceManager& resourceManager, 
                                 AppConfig& config) :
    State(screen, resourceManager, config),
    m_menu(screen, resourceManager),
    m_currentLevelNumber(1),
    m_unlockedLevel(1),
    m_transitionStateInfo(LevelSelectStateId)
{
    loadLevelInfos();
    auto captionName = m_levelInfos.find(1);
    if(captionName != end(m_levelInfos))
        m_menu.getLabel(LevelSelectMenu::LABEL_CAPTION).setText(captionName->second.name);

    auto preview = m_levelInfos.find(1);
    if(preview != end(m_levelInfos))
        m_menu.setLevelPreview(*preview->second.preview.getSprite().getTexture(), preview->second.preview.getTextureRect());

    updateRightButton();

    m_menu.hideLeftButton(true);
}

LevelSelectState::~LevelSelectState()
{
}

void LevelSelectState::onEnter(const EnterStateInformation* enterInformation, const double time)
{
    State::onEnter(enterInformation, time);

    m_unlockedLevel = m_config.get<int>("UnlockedLevel");

    updateButtons();
    m_menu.update(m_screen, getPassedTime());
}

StateChangeInformation LevelSelectState::update(const double time)
{ 
    if(State::isPaused())
        return StateChangeInformation::Empty();

    int clicked = -1;
    updateTime(time);
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.update(m_screen, getPassedTime());

    if(clicked == LevelSelectMenu::BUTTON_MAIN_MENU)
    {
        m_transitionStateInfo.m_followingState = MainMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    if(clicked == LevelSelectMenu::BUTTON_SHOW_DETAILS && !(m_currentLevelNumber > m_unlockedLevel))
    {
        m_loadLevelStateInfo.m_levelNumber = m_currentLevelNumber;
        m_transitionStateInfo.m_followingState = LoadLevelStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_loadLevelStateInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    if(clicked == LevelSelectMenu::BUTTON_LEFT)
    {
        if(m_levelInfos.find(m_currentLevelNumber - 1) != end(m_levelInfos))
        {
            m_currentLevelNumber--;
            m_menu.getLabel(LevelSelectMenu::LABEL_CAPTION).setText(m_levelInfos.find(m_currentLevelNumber)->second.name);
            auto preview = m_levelInfos.find(m_currentLevelNumber);
            if(preview != end(m_levelInfos))
                m_menu.setLevelPreview(*preview->second.preview.getSprite().getTexture(), preview->second.preview.getTextureRect());
        }
    }
    if(clicked == LevelSelectMenu::BUTTON_RIGHT)
    {
        if(m_levelInfos.find(m_currentLevelNumber + 1) != end(m_levelInfos))
        {
            m_currentLevelNumber++;
            m_menu.getLabel(LevelSelectMenu::LABEL_CAPTION).setText(m_levelInfos.find(m_currentLevelNumber)->second.name);
            auto preview = m_levelInfos.find(m_currentLevelNumber);
            if(preview != end(m_levelInfos))
                m_menu.setLevelPreview(*preview->second.preview.getSprite().getTexture(), preview->second.preview.getTextureRect());
        }
    }

    updateButtons();

    return StateChangeInformation::Empty();
}

void LevelSelectState::doDraw(const DrawParameter& params)
{    
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 255));
    params.getTarget().draw(whiteRect);

    m_menu.draw(params);
}

void LevelSelectState::loadLevelInfos()
{
    tinyxml2::XMLDocument doc;
    std::string file = "";
    std::unordered_map<int, std::string> fileNames = State::getResourceManager().getFileNames();

    int i = 0;
    for(auto it = std::begin(fileNames); it != std::end(fileNames); ++it)
    {
        Level level("", it->first, getResourceManager().getSubScope("menus"), getConfig(), true);
        m_levelInfos[it->first] = level.getInfo();
    }
    getResourceManager().purge("menus");
}

void LevelSelectState::updateLeftButton()
{
    auto preview = m_levelInfos.find(m_currentLevelNumber - 1);
    if(preview != end(m_levelInfos))
    {
        sf::Sprite sprite = preview->second.preview.getSprite();
        auto& b = m_menu.getButton(LevelSelectMenu::BUTTON_LEFT);
        b.changeIdleSprite(sprite);
        b.changeHoverSprite(sprite);
        b.changePressedSprite(sprite);
    }
    m_menu.hideLeftButton(false);
}

void LevelSelectState::updateRightButton()
{
    auto preview = m_levelInfos.find(m_currentLevelNumber + 1);
    if(preview != end(m_levelInfos))
    {
        sf::Sprite sprite = preview->second.preview.getSprite();
        auto& b = m_menu.getButton(LevelSelectMenu::BUTTON_RIGHT);
        b.changeIdleSprite(sprite);
        b.changeHoverSprite(sprite);
        b.changePressedSprite(sprite);
    }
    m_menu.hideRightButton(false);
}

void LevelSelectState::updateButtons()
{
    if(m_levelInfos.find(m_currentLevelNumber - 1) == end(m_levelInfos))
        m_menu.hideLeftButton(true);
    else
        updateLeftButton();

    if(m_levelInfos.find(m_currentLevelNumber + 1) == end(m_levelInfos) ||
       (m_unlockedLevel < (m_currentLevelNumber + 1)))
        m_menu.hideRightButton(true);
    else
        updateRightButton();
}
