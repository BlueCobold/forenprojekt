#include "LevelSelectState.hpp"

#include "../gui/Button.hpp"
#include "../resources/Config.hpp"
#include "../rendering/transitions/RandomTransition.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

#include <fstream> 
#include <tinyxml2.h>

LevelSelectState::LevelSelectState(sf::RenderWindow& screen, 
                                 ResourceManager& resourceManager, 
                                 AppConfig& config) :
    State(screen, resourceManager, config),
    m_menu(sf::Vector2f(0, 0), screen, resourceManager),
    m_currentLevelNumber(1),
    m_unlockedLevel(1)
{
    loadLevelInfos();
    auto captionName = m_levelNames.find(1);
    if(captionName != end(m_levelNames))
        m_menu.setCaption(captionName->second);

    auto texturRect = m_textureCoordinates.find(1);
    if(texturRect != end(m_textureCoordinates))
        m_menu.setLevelTextureRect(m_textureCoordinates.find(m_currentLevelNumber)->second);

    updateRightButton();

    m_menu.hideLeftButton(true);
}

LevelSelectState::~LevelSelectState()
{
}

void LevelSelectState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    State::onEnter(enterInformation, time);

    m_menu.updateLayout();

    m_unlockedLevel = m_config.get<int>("UnlockedLevel");
}

StateChangeInformation LevelSelectState::update(const float time)
{ 
    if(State::isPaused())
        return StateChangeInformation::Empty();

    int clicked = -1;
    updateTime(time);
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.update(m_screen, getPassedTime());

    if(clicked == LevelSelectMenu::BUTTON_MAIN_MENU)
    {
        m_stateInfo.m_level = nullptr;
        m_stateInfo.m_prepareOnly = false;
        m_transitionStateInfo.m_followingState = MainMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;
        m_transitionStateInfo.m_comeFromeState = LevelSelectStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    if(clicked == LevelSelectMenu::BUTTON_SHOW_DETAILS && !(m_currentLevelNumber > m_unlockedLevel))
    {
        m_stateInfo.m_level = nullptr;
        m_stateInfo.m_prepareOnly = false;
        m_stateInfo.m_levelNumber = m_currentLevelNumber;
        m_transitionStateInfo.m_followingState = LoadLevelStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;
        m_transitionStateInfo.m_comeFromeState = LevelSelectStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    if(clicked == LevelSelectMenu::BUTTON_LEFT)
    {
        if(m_levelNames.find(m_currentLevelNumber - 1) != end(m_levelNames))
        {
            m_currentLevelNumber--;
            m_menu.setCaption(m_levelNames.find(m_currentLevelNumber)->second);
            m_menu.setLevelTextureRect(m_textureCoordinates.find(m_currentLevelNumber)->second);
        }
    }
    if(clicked == LevelSelectMenu::BUTTON_RIGHT)
    {
        if(m_levelNames.find(m_currentLevelNumber + 1) != end(m_levelNames))
        {
            m_currentLevelNumber++;
            m_menu.setCaption(m_levelNames.find(m_currentLevelNumber)->second);
            m_menu.setLevelTextureRect(m_textureCoordinates.find(m_currentLevelNumber)->second);
        }
    }

    if(m_levelNames.find(m_currentLevelNumber - 1) == end(m_levelNames))
        m_menu.hideLeftButton(true);
    else
        updateLeftButton();

    if(m_levelNames.find(m_currentLevelNumber + 1) == end(m_levelNames) ||
       (m_unlockedLevel < (m_currentLevelNumber + 1)))
        m_menu.hideRightButton(true);
    else
        updateRightButton();

    return StateChangeInformation::Empty();
}

void LevelSelectState::draw(const DrawParameter& params)
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

    for(auto it = std::begin(fileNames); it != std::end(fileNames); ++it)
    {
        file = "res/level/" + it->second;

        doc.LoadFile(file.c_str());
        if(doc.Error()) // Error while loading file
        {
            auto error = utility::replace(utility::translateKey("InvalidXml"), file + " [" + doc.GetErrorStr1() +"]");
            if(doc.GetErrorStr2())
                error += std::string(" / ") + doc.GetErrorStr2();
            throw std::runtime_error(error);
        }
        auto levelinfo = doc.FirstChildElement("level")->FirstChildElement("levelinfo");

        std::string name = levelinfo->Attribute("name");

        sf::IntRect textureRect(levelinfo->FirstChildElement("infoimage")->IntAttribute("scrx"),
                                levelinfo->FirstChildElement("infoimage")->IntAttribute("scry"),
                                levelinfo->FirstChildElement("infoimage")->IntAttribute("width"),
                                levelinfo->FirstChildElement("infoimage")->IntAttribute("height"));
        m_levelNames[it->first] = name;
        m_textureCoordinates[it->first] = textureRect;
    }
}

void LevelSelectState::updateLeftButton()
{
    sf::Sprite sprite(*getResourceManager().getTexture("GuiElements"), m_textureCoordinates.find(m_currentLevelNumber - 1)->second);
    m_menu.changeIdleSprite(LevelSelectMenu::BUTTON_LEFT, sprite);
    m_menu.changeHoverSprite(LevelSelectMenu::BUTTON_LEFT, sprite);
    m_menu.changePressedSprite(LevelSelectMenu::BUTTON_LEFT, sprite);
    m_menu.hideLeftButton(false);
}

void LevelSelectState::updateRightButton()
{
    sf::Sprite sprite(*getResourceManager().getTexture("GuiElements"), m_textureCoordinates.find(m_currentLevelNumber + 1)->second);
    m_menu.changeIdleSprite(LevelSelectMenu::BUTTON_RIGHT, sprite);
    m_menu.changeHoverSprite(LevelSelectMenu::BUTTON_RIGHT, sprite);
    m_menu.changePressedSprite(LevelSelectMenu::BUTTON_RIGHT, sprite);
    m_menu.hideRightButton(false);
}