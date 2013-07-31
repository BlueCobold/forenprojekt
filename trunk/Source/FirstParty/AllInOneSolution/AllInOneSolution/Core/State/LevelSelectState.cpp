#include "LevelSelectState.hpp"
#include "../resources/Config.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <fstream> 
#include <tinyxml2.h>

LevelSelectState::LevelSelectState(sf::RenderWindow& screen, 
                                 ResourceManager& resourceManager, 
                                 AppConfig& config) :
    State(screen, resourceManager, config),
    m_menu(sf::Vector2f(0, 0), screen, resourceManager)
{
    loadLevelNames();
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
}

StateChangeInformation LevelSelectState::update(const float time)
{ 
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
void LevelSelectState::loadLevelNames()
{
    tinyxml2::XMLDocument doc;
    std::string file = "";
    for(int i = 1; i<10; ++i)
    {
        if(i < 10)
            file = "res/level/00" + utility::toString(i) + ".lvl";
        else if (i > 9 && i < 100)
            file = "res/level/0" + utility::toString(i) + ".lvl";

        std::ifstream fileTest(file); 
        if(!fileTest)
            break;

        doc.LoadFile(file.c_str());

        std::string name = doc.FirstChildElement("level")->FirstChildElement("levelinfo")->Attribute("name");
        m_levelNames[i] = name;
    }
}