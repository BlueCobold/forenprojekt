#include "LevelSelectState.hpp"

#include "../gui/Button.hpp"
#include "../resources/Config.hpp"

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
}

LevelSelectState::~LevelSelectState()
{
}

void LevelSelectState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    State::onEnter(enterInformation,time);

    m_menu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_menu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_menu.getSize().y / 2.f));

    m_unlockedLevel = m_config.get<int>("UnlockedLevel");
}

StateChangeInformation LevelSelectState::update(const float time)
{ 
    if(State::isPaused())
        return StateChangeInformation::Empty();

    m_menu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_menu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_menu.getSize().y / 2.f));

    int clicked = -1;
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.update(m_screen);

    if(clicked == LevelSelectMenu::BUTTON_MAIN_MENU)
    {
        m_stateInfo.m_level = nullptr;
        m_stateInfo.m_prepareOnly = false;
        m_transitionStateInfo.m_followingState = MainMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    if(clicked == LevelSelectMenu::BUTTON_SHOW_DETAILS && !(m_currentLevelNumber > m_unlockedLevel))
    {
        m_stateInfo.m_level = nullptr;
        m_stateInfo.m_prepareOnly = false;
        m_stateInfo.m_levelNumber = m_currentLevelNumber;
        m_transitionStateInfo.m_followingState = LoadLevelStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_stateInfo;
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
    {
        sf::Sprite sprite(*getResourceManager().getTexture("GuiElements"), m_textureCoordinates.find(m_currentLevelNumber-1)->second);
        m_menu.changeIdleSprite(LevelSelectMenu::BUTTON_LEFT, sprite);
        m_menu.changeHoverSprite(LevelSelectMenu::BUTTON_LEFT, sprite);
        m_menu.changePressedSprite(LevelSelectMenu::BUTTON_LEFT, sprite);
        m_menu.hideLeftButton(false);
    }
    if(m_levelNames.find(m_currentLevelNumber + 1) == end(m_levelNames) ||
       (m_unlockedLevel < (m_currentLevelNumber + 1)))
        m_menu.hideRightButton(true);
    else
    {
        sf::Sprite sprite(*getResourceManager().getTexture("GuiElements"), m_textureCoordinates.find(m_currentLevelNumber+1)->second);
        m_menu.changeIdleSprite(LevelSelectMenu::BUTTON_RIGHT, sprite);
        m_menu.changeHoverSprite(LevelSelectMenu::BUTTON_RIGHT, sprite);
        m_menu.changePressedSprite(LevelSelectMenu::BUTTON_RIGHT, sprite);
        m_menu.hideRightButton(false);
    }

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
    for(int i = 1; i < 100; ++i)
    {
        if(i < 10)
            file = "res/level/00" + utility::toString(i) + ".lvl";
        else if (i > 9 && i < 100)
            file = "res/level/0" + utility::toString(i) + ".lvl";

        std::ifstream fileTest(file); 
        if(!fileTest)
            break;

        doc.LoadFile(file.c_str());
        auto levelinfo = doc.FirstChildElement("level")->FirstChildElement("levelinfo");

        std::string name = levelinfo->Attribute("name");

        sf::IntRect textureRect(levelinfo->FirstChildElement("infoimage")->IntAttribute("scrx"),
                                levelinfo->FirstChildElement("infoimage")->IntAttribute("scry"),
                                levelinfo->FirstChildElement("infoimage")->IntAttribute("width"),
                                levelinfo->FirstChildElement("infoimage")->IntAttribute("height"));
        m_levelNames[i] = name;
        m_textureCoordinates[i] = textureRect;
    }
}