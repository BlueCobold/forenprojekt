#include "LevelSelectState.hpp"

#include "../gui/Button.hpp"
#include "../resources/Config.hpp"
#include "../rendering/transitions/RandomTransition.hpp"
#include "../MacHelper.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

#include <fstream> 
#include <tinyxml2.h>

LevelSelectState::LevelSelectState(sf::RenderWindow& screen, 
                                 ResourceManager& resourceManager, 
                                 AppConfig& config) :
    State(screen, resourceManager, config),
    m_menu(screen, resourceManager),
    m_currentLevelNumber(1),
    m_unlockedLevel(1)
{
    loadLevelInfos();
    auto captionName = m_levelNames.find(1);
    if(captionName != end(m_levelNames))
        m_menu.setCaption(captionName->second);

    auto preview = m_textureCoordinates.find(1);
    if(preview != end(m_textureCoordinates))
        m_menu.setLevelPreview(*preview->second.texture, preview->second.rect);

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

    updateButtons();
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
        m_loadLevelStateInfo.m_level = nullptr;
        m_loadLevelStateInfo.m_prepareOnly = false;
        m_loadLevelStateInfo.m_levelNumber = m_currentLevelNumber;
        m_loadLevelStateInfo.m_directPlay = false;
        m_transitionStateInfo.m_followingState = LoadLevelStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_loadLevelStateInfo;
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
            auto preview = m_textureCoordinates.find(m_currentLevelNumber);
            if(preview != end(m_textureCoordinates))
                m_menu.setLevelPreview(*preview->second.texture, preview->second.rect);
        }
    }
    if(clicked == LevelSelectMenu::BUTTON_RIGHT)
    {
        if(m_levelNames.find(m_currentLevelNumber + 1) != end(m_levelNames))
        {
            m_currentLevelNumber++;
            m_menu.setCaption(m_levelNames.find(m_currentLevelNumber)->second);
            auto preview = m_textureCoordinates.find(m_currentLevelNumber);
            if(preview != end(m_textureCoordinates))
                m_menu.setLevelPreview(*preview->second.texture, preview->second.rect);
        }
    }

    updateButtons();

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
        file = resourcePath() + "res/level/" + it->second;

        doc.LoadFile(file.c_str());
        if(doc.Error()) // Error while loading file
        {
            auto msg = std::string(doc.GetErrorStr1() ? doc.GetErrorStr1() : "");
            auto error = utility::replace(utility::translateKey("InvalidXml"), file + " [" + msg +"]");
            if(doc.GetErrorStr2())
                error += std::string(" / ") + doc.GetErrorStr2();
            throw std::runtime_error(error);
        }
        auto levelinfo = doc.FirstChildElement("level")->FirstChildElement("levelinfo");

        std::string name = levelinfo->Attribute("name");

        PreviewInfo preview;
        if(auto info = levelinfo->FirstChildElement("infoimage"))
        {
            SpriteSheet::SpriteData sprite;
            auto spriteName = info->Attribute("sprite");
            auto sheetName = spriteName ? info->Attribute("spriteSheet") : nullptr;
            auto sheet = sheetName ? getResourceManager().getSpriteSheet(sheetName) : nullptr;
            if(sheet != nullptr)
                sprite = sheet->get(spriteName);
            if(info->Attribute("srcx"))
                sprite.x = info->IntAttribute("srcx");
            if(info->Attribute("srcy"))
                sprite.y = info->IntAttribute("srcy");
            if(info->Attribute("width"))
                sprite.width = info->IntAttribute("width");
            if(info->Attribute("height"))
                sprite.height = info->IntAttribute("height");
            preview.texture = getResourceManager().getTexture(sheet ? sheet->getTextureName() : "GuiElements");
            preview.rect = sf::IntRect(sprite.x, sprite.y, sprite.width, sprite.height);
        }
        m_textureCoordinates[it->first] = preview;
        m_levelNames[it->first] = name;
    }
}

void LevelSelectState::updateLeftButton()
{
    auto preview = m_textureCoordinates.find(m_currentLevelNumber - 1);
    if(preview != end(m_textureCoordinates))
    {
        sf::Sprite sprite(*preview->second.texture, preview->second.rect);
        m_menu.changeIdleSprite(LevelSelectMenu::BUTTON_LEFT, sprite);
        m_menu.changeHoverSprite(LevelSelectMenu::BUTTON_LEFT, sprite);
        m_menu.changePressedSprite(LevelSelectMenu::BUTTON_LEFT, sprite);
    }
    m_menu.hideLeftButton(false);
}

void LevelSelectState::updateRightButton()
{
    auto preview = m_textureCoordinates.find(m_currentLevelNumber + 1);
    if(preview != end(m_textureCoordinates))
    {
        sf::Sprite sprite(*preview->second.texture, preview->second.rect);
        m_menu.changeIdleSprite(LevelSelectMenu::BUTTON_RIGHT, sprite);
        m_menu.changeHoverSprite(LevelSelectMenu::BUTTON_RIGHT, sprite);
        m_menu.changePressedSprite(LevelSelectMenu::BUTTON_RIGHT, sprite);
    }
    m_menu.hideRightButton(false);
}

void LevelSelectState::updateButtons()
{
    if(m_levelNames.find(m_currentLevelNumber - 1) == end(m_levelNames))
        m_menu.hideLeftButton(true);
    else
        updateLeftButton();

    if(m_levelNames.find(m_currentLevelNumber + 1) == end(m_levelNames) ||
       (m_unlockedLevel < (m_currentLevelNumber + 1)))
        m_menu.hideRightButton(true);
    else
        updateRightButton();
}