 #include "LevelPreviewState.hpp"

#include "../gui/Button.hpp"
#include "../gui/CheckBox.hpp"
#include "../model/Level.hpp"
#include "../resources/AppConfig.hpp"
#include "../rendering/transitions/RandomTransition.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

LevelPreviewState::LevelPreviewState(sf::RenderWindow& screen, 
                                 ResourceManager& resourceManager, 
                                 AppConfig& config) :
    State(screen, resourceManager, config),
    m_menu(sf::Vector2f(0, 0), screen, resourceManager),
    m_HUD(resourceManager, config),
    m_level(nullptr),
    m_levelUpdated(false),
    m_levelNumber(0)
{
    m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TIMEATTACKMODE).setChecked(false);
    m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TARGETMODE).setChecked(true);
}

LevelPreviewState::~LevelPreviewState()
{
}

void LevelPreviewState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    m_levelUpdated = false;
    State::onEnter(enterInformation, time);

    m_level = enterInformation->m_level;

    m_HUD.onEnter(m_level);

    m_menu.setLevelInfo(m_level->getLevelName(), m_level->getTotalTime(), m_level->getRemainingBall());
    m_menu.setCoinToolTipText(utility::replace(utility::translateKey("tooltip_coins"), 
                                           utility::toString(m_config.get<int>("coins"))));

    if(m_config.get<unsigned int>("UnlockedLevel") == m_level->number())
    {
        m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TIMEATTACKMODE).setToolTipText(utility::translateKey("tooltip_preview_notimeattack"));
        m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TIMEATTACKMODE).setChecked(false);
        m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TARGETMODE).setChecked(true);
    }
    else
        m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TIMEATTACKMODE).setToolTipText(utility::translateKey("tooltip_preview_timeattack"));

    m_levelNumber = enterInformation->m_levelNumber;

    m_menu.updateLayout();
}

StateChangeInformation LevelPreviewState::update(const float time)
{
    if(State::isPaused())
        return StateChangeInformation::Empty();

    int clicked = -1;
    updateTime(time);
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.update(m_screen, getPassedTime());

    if(m_level->isTimeAttackMode())
    {
        if(m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TARGETMODE).getChecked())
        {
            m_level->setTimeAttackMode(false);
            m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TIMEATTACKMODE).setChecked(false);
        }
        else
            m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TIMEATTACKMODE).setChecked(true);
    }
    else
    {
        if(m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TIMEATTACKMODE).getChecked() &&
           m_config.get<unsigned int>("UnlockedLevel") > m_level->number())
        {
            m_level->setTimeAttackMode(true);
            m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TARGETMODE).setChecked(false);
        }
        else
        {
            m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TARGETMODE).setChecked(true);
            m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TIMEATTACKMODE).setChecked(false);
        }
    }
    m_menu.setLevelInfo(m_level->getLevelName(), m_level->getRemainigTime(), m_level->getRemainingBall());
    if(clicked == LevelPreviewMenu::BUTTON_START)
    {
        m_level->setTimeAttackMode(m_menu.getCheckbox(10).getChecked());
        m_playStateInfo.m_prepareOnly = false;
        m_playStateInfo.m_returnFromPause = false;
        m_playStateInfo.m_level = m_level;
        m_playStateInfo.m_levelNumber = m_levelNumber;
        m_transitionStateInfo.m_followingState = PlayStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
        m_transitionStateInfo.m_comeFromeState = LevelPreviewStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == LevelPreviewMenu::BUTTON_MAIN_MENU)
    {
        m_playStateInfo.m_prepareOnly = false;
        m_playStateInfo.m_returnFromPause = false;
        m_playStateInfo.m_level = m_level;
        m_playStateInfo.m_levelNumber = m_levelNumber;
        m_transitionStateInfo.m_followingState = MainMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
        m_transitionStateInfo.m_comeFromeState = LevelPreviewStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == LevelPreviewMenu::BUTTON_COINS)
    {
        m_coinShopStateInfo.m_level = m_level;
        m_coinShopStateInfo.m_prepareOnly = false;
        m_coinShopStateInfo.m_comeFromState = LevelPreviewStateId;
        m_coinShopStateInfo.m_levelNumber = m_levelNumber;
        m_transitionStateInfo.m_followingState = CoinShopStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_coinShopStateInfo;
        m_transitionStateInfo.m_comeFromeState = LevelPreviewStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == LevelPreviewMenu::BUTTON_HIGHSCORE)
    {
        m_highScoreInfo.m_level = m_level;
        m_highScoreInfo.m_comeFromState = LevelPreviewStateId;
        m_highScoreInfo.m_levelNumber = m_levelNumber;
        m_transitionStateInfo.m_followingState = HighScoreStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_highScoreInfo;
        m_transitionStateInfo.m_comeFromeState = LevelPreviewStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }

    return StateChangeInformation::Empty();
}

void LevelPreviewState::draw(const DrawParameter& params)
{
    if(m_level != nullptr)
    {
        m_level->adjustView(params.getTarget());
        if(!m_levelUpdated)
        {
            m_level->update(0, params.getTarget());
            m_levelUpdated = true;
            m_HUD.update(m_level, getPassedTime());
        }
        m_level->draw(params);
        m_HUD.draw(params);
    }

    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 128));
    params.getTarget().draw(whiteRect);

    m_menu.draw(params);
}