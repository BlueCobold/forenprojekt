#include "PauseState.hpp"

#include "../gui/Button.hpp"
#include "../gui/MenuSprite.hpp"
#include "../Input.hpp"
#include "../model/Level.hpp"
#include "../resources/Config.hpp"
#include "../resources/ResourceManager.hpp"

#include <memory>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>

PauseState::PauseState(sf::RenderWindow& screen,
                       ResourceManager& resourceManager,
                       AppConfig& config) :
    State(screen, resourceManager, config),
    m_screen(screen),
    m_gestures(screen),
    m_background(nullptr),
    m_menu(screen, resourceManager),
    m_HUD(resourceManager, config),
    m_level(nullptr),
    m_levelNumber(0),
    m_transitionStateInfo(PauseStateId),
    m_optionStateInfo(PauseStateId)
{
    std::vector<GesturePart> parts;
    parts.push_back(GesturePart(-0.349066f, 0.349066f));
    parts.push_back(GesturePart(2.79253f, -2.79253f));
    parts.push_back(GesturePart(-1.91986f, -1.22173f));
    parts.push_back(GesturePart(1.22173f, 1.91986f));
    m_gestures.addGesture(Gesture(5, parts), [&](){
        m_level->addBall();
    });
}

PauseState::~PauseState()
{
}

void PauseState::onEnter(const EnterStateInformation* enterInformation, const double time)
{
    const EnterPauseStateInformation* info = dynamic_cast<const EnterPauseStateInformation*>(enterInformation);
    if(info->m_level)
        m_level = info->m_level;
    m_levelNumber = info->m_levelNumber;

    m_menu.getSprite(PauseMenu::IMAGE_PREVIEW).setSprite(m_level->getInfo().preview);

    State::onEnter(enterInformation, time);
    m_HUD.onEnter(m_level);
    m_HUD.restartAt(getPassedTime());

    m_playStateInfo.m_levelNumber = enterInformation->m_levelNumber;
    m_optionStateInfo.m_levelNumber = enterInformation->m_levelNumber;

    m_menu.setCoinToolTipText("tooltip_coins", utility::toString(m_config.get<int>("coins")));
    m_menu.update(m_screen, getPassedTime());
}

StateChangeInformation PauseState::update(const double time)
{
    if(State::isPaused())
        return StateChangeInformation::Empty();

    updateTime(time);

    m_gestures.process();

    int clicked = -1;
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.update(m_screen, getPassedTime());
    
    if(utility::Keyboard.isKeyReleased(sf::Keyboard::P)
        || utility::Keyboard.isKeyReleased(sf::Keyboard::Pause)
        || utility::Keyboard.isKeyReleased(sf::Keyboard::Escape)
        || PauseMenu::BUTTON_CONTINUE == clicked)
    {
        m_playStateInfo.m_returnFromPause = true;
        m_playStateInfo.m_level = m_level;
        m_transitionStateInfo.m_level = m_level;
        m_transitionStateInfo.m_followingState = PlayStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == PauseMenu::BUTTON_OPTIONS)
    {
        m_optionStateInfo.m_comeFromState = PauseStateId;
        m_optionStateInfo.m_level = m_level;
        m_optionStateInfo.m_prepareOnly = false;
        m_transitionStateInfo.m_level = m_level;
        m_transitionStateInfo.m_followingState = OptionMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_optionStateInfo;
        m_transitionStateInfo.m_comeFromeState = PauseStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == PauseMenu::BUTTON_RESTART_LEVEL)
    {
#ifdef LEVELTESTING
        if(m_level->number() == 0)
        {
            m_loadLevelStateInfo.m_file = m_level->getFileName();
            m_loadLevelStateInfo.m_level = nullptr;
            m_loadLevelStateInfo.m_directPlay = true;
            m_loadLevelStateInfo.m_levelNumber = 0;
        }
        else
        {
            m_loadLevelStateInfo.m_file = "";
            m_loadLevelStateInfo.m_level = m_level;
            m_loadLevelStateInfo.m_directPlay = false;
            m_loadLevelStateInfo.m_levelNumber = m_playStateInfo.m_levelNumber;
        }
        m_loadLevelStateInfo.m_prepareOnly = false;
        m_transitionStateInfo.m_followingState = LoadLevelStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_loadLevelStateInfo;
        m_transitionStateInfo.m_comeFromeState = PauseStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
#else
        m_loadLevelStateInfo.m_prepareOnly = true;
        m_loadLevelStateInfo.m_directPlay = false;
        m_loadLevelStateInfo.m_level = m_level;
        m_loadLevelStateInfo.m_levelNumber = m_playStateInfo.m_levelNumber;
        m_transitionStateInfo.m_level = m_level;
        m_transitionStateInfo.m_followingState = LoadLevelStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_loadLevelStateInfo;
        m_transitionStateInfo.m_comeFromeState = PauseStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
#endif
    }
    else if(clicked == PauseMenu::BUTTON_MAIN_MENU)
    {
        m_playStateInfo.m_prepareOnly = true;
        m_playStateInfo.m_returnFromPause = true;
        m_playStateInfo.m_level = m_level;
        m_transitionStateInfo.m_level = m_level;
        m_transitionStateInfo.m_followingState = MainMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
        m_transitionStateInfo.m_comeFromeState = PauseStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == PauseMenu::BUTTON_COINS)
    {
        m_coinShopStateInfo.m_level = m_level;
        m_coinShopStateInfo.m_prepareOnly = false;
        m_coinShopStateInfo.m_comeFromState = PauseStateId;
        m_coinShopStateInfo.m_levelNumber = m_levelNumber;
        m_transitionStateInfo.m_followingState = CoinShopStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_coinShopStateInfo;
        m_transitionStateInfo.m_comeFromeState = PauseStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    return StateChangeInformation::Empty();
}

void PauseState::doDraw(const DrawParameter& params)
{
    if(m_level != nullptr)
    {
        m_level->adjustView(params);
        m_level->draw(params);
    }
    m_HUD.update(m_level, getPassedTime());
    m_HUD.draw(params);

    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 128));
    params.getTarget().draw(whiteRect);

    m_menu.draw(params);
}
