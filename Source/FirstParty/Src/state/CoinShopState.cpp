#include "CoinShopState.hpp"

#include "../gui/Button.hpp"
#include "../Input.hpp"
#include "../resources/Config.hpp"
#include "../resources/ResourceManager.hpp"
#include "../model/Level.hpp"
#include "EnterStateInformation.hpp"
#include "../resources/AppConfig.hpp"
#include "../resources/AchievementManager.hpp"

#include <memory>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>

CoinShopState::CoinShopState(sf::RenderWindow& screen,
                             ResourceManager& resourceManager,
                             AppConfig& config,
                             AchievementManager& achievementManager) :
    State(screen, resourceManager, config),
    m_menu(screen, resourceManager),
    m_HUD(resourceManager, config),
    m_clicked(-1),
    m_transitionStateInfo(CoinShopStateId),
    m_achievementManager(achievementManager)
{
    auto buttonFunc = [&](const Button& sender){ m_clicked = sender.getId(); };
    m_menu.getButton(CoinShopMenu::BUTTON_GRAVITY_PLUS).registerOnPressed(buttonFunc);
    m_menu.getButton(CoinShopMenu::BUTTON_INVULNERABLE_PLUS).registerOnPressed(buttonFunc);
    m_menu.getButton(CoinShopMenu::BUTTON_EXTRA_TIME_PLUS).registerOnPressed(buttonFunc);
    m_menu.getButton(CoinShopMenu::BUTTON_EXTRA_BALL_PLUS).registerOnPressed(buttonFunc);
    m_menu.getButton(CoinShopMenu::BUTTON_GRAVITY_MINUS).registerOnPressed(buttonFunc);
    m_menu.getButton(CoinShopMenu::BUTTON_INVULNERABLE_MINUS).registerOnPressed(buttonFunc);
    m_menu.getButton(CoinShopMenu::BUTTON_EXTRA_TIME_MINUS).registerOnPressed(buttonFunc);
    m_menu.getButton(CoinShopMenu::BUTTON_EXTRA_BALL_MINUS).registerOnPressed(buttonFunc);
    m_menu.getButton(CoinShopMenu::BUTTON_CLOSE).registerOnPressed(buttonFunc);
}

CoinShopState::~CoinShopState()
{
}

void CoinShopState::onEnter(const EnterStateInformation* enterInformation, const double time)
{
    const EnterCoinShopStateInformation* info = dynamic_cast<const EnterCoinShopStateInformation*>(enterInformation);
    State::onEnter(info, time);

    m_level = info->m_level;
    m_levelNumber = info->m_levelNumber;
    m_transitionStateInfo.m_followingState = info->m_comeFromState;

    m_menu.setGoodyCharges(Goody::GravityGoody, m_config.get<int>("goodygravity"));
    m_menu.setGoodyCharges(Goody::InvulnerableGoody, m_config.get<int>("goodyinvulnerable"));
    m_menu.setGoodyCharges(Goody::ExtraBallGoody, m_config.get<int>("goodyextraball"));
    m_menu.setGoodyCharges(Goody::ExtraTimeGoody, m_config.get<int>("goodyextratime"));

    updateButtons();
    m_menu.update(m_screen, getPassedTime());
}

void CoinShopState::buy(const Goody::Type type, const std::string& propertyKey)
{
    int cost = m_menu.getBuyCost(type);
    int coins = m_config.get<int>("coins");
    int charge = m_config.get<int>(propertyKey);
    if(coins >= cost)
    {
        charge++;
        m_menu.setGoodyCharges(type, charge);
        m_config.set<int>(propertyKey, charge);
        m_config.set<int>("coins", coins - cost);
        if(Goody::InvulnerableGoody == type)
            m_achievementManager.addValueTo(Achievement::Buy, Achievement::InSum, Achievement::InvulnerableGoody, 1);
        else if(Goody::ExtraBallGoody == type)
            m_achievementManager.addValueTo(Achievement::Buy, Achievement::InSum, Achievement::ExtraBallGoody, 1);
        else if(Goody::ExtraTimeGoody == type)
            m_achievementManager.addValueTo(Achievement::Buy, Achievement::InSum, Achievement::ExtraTimeGoody, 1);
        else if(Goody::GravityGoody == type)
            m_achievementManager.addValueTo(Achievement::Buy, Achievement::InSum, Achievement::GravityGoody, 1);
        else
            throw std::runtime_error(utility::translateKey("@InvalidGoody"));

        m_achievementManager.addValueTo(Achievement::Buy, Achievement::InSum, Achievement::GoodyGenaral, 1);
        m_achievementManager.addValueTo(Achievement::Buy, Achievement::InCash, Achievement::GoodyGenaral, cost);
        m_achievementManager.saveValues();
    }
}

void CoinShopState::sell(const Goody::Type type, const std::string& propertyKey)
{
    int refund = m_menu.getSellRefund(type);
    int coins = m_config.get<int>("coins");
    int charge = m_config.get<int>(propertyKey);
    if(charge != 0)
    {
        charge--;
        m_menu.setGoodyCharges(type, charge);
        m_config.set<int>(propertyKey, charge);
        m_config.set<int>("coins", coins + refund);

        if(Goody::InvulnerableGoody == type)
            m_achievementManager.addValueTo(Achievement::Sell, Achievement::InSum, Achievement::InvulnerableGoody, 1);
        else if(Goody::ExtraBallGoody == type)
            m_achievementManager.addValueTo(Achievement::Sell, Achievement::InSum, Achievement::ExtraBallGoody, 1);
        else if(Goody::ExtraTimeGoody == type)
            m_achievementManager.addValueTo(Achievement::Sell, Achievement::InSum, Achievement::ExtraTimeGoody, 1);
        else if(Goody::GravityGoody == type)
            m_achievementManager.addValueTo(Achievement::Sell, Achievement::InSum, Achievement::GravityGoody, 1);
        else
            throw std::runtime_error(utility::translateKey("@InvalidGoody"));

        m_achievementManager.addValueTo(Achievement::Sell, Achievement::InSum, Achievement::GoodyGenaral, 1);
        m_achievementManager.addValueTo(Achievement::Sell, Achievement::InCash, Achievement::GoodyGenaral, refund);
        m_achievementManager.saveValues();
    }
}

StateChangeInformation CoinShopState::update(const double time)
{
    if(State::isPaused())
        return StateChangeInformation::Empty();

    updateTime(time);

    m_clicked = -1;
    m_menu.update(m_screen, getPassedTime());
    updateButtons();

    if(utility::Keyboard.isKeyReleased(sf::Keyboard::Escape))
        m_clicked = CoinShopMenu::BUTTON_CLOSE;

    switch(m_clicked)
    {
    case CoinShopMenu::BUTTON_CLOSE:
        if(m_transitionStateInfo.m_followingState == LevelPreviewStateId)
        {
            m_levelPreviewInfo.m_returnFromPause = false;
            m_transitionStateInfo.m_onEnterInformation = &m_levelPreviewInfo;
        }
        else if(m_transitionStateInfo.m_followingState == PauseStateId)
            m_transitionStateInfo.m_onEnterInformation = &m_pauseStateInfo;

        m_levelPreviewInfo.m_level = m_level;
        m_levelPreviewInfo.m_levelNumber = m_levelNumber;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    case CoinShopMenu::BUTTON_GRAVITY_PLUS:
        {
            int cost = m_menu.getBuyCost(Goody::GravityGoody);
            int coins = m_config.get<int>("coins");
            int charge = m_config.get<int>("goodygravity");
            if(coins >= cost && charge == 0)
            {
                charge = -1;
                m_menu.setGoodyCharges(Goody::GravityGoody, charge);
                m_config.set<int>("goodygravity", charge);
                m_config.set<int>("coins", coins - cost);
            }
        }
        break;
    case CoinShopMenu::BUTTON_INVULNERABLE_PLUS:
        buy(Goody::InvulnerableGoody, "goodyinvulnerable");
        break;
    case CoinShopMenu::BUTTON_EXTRA_BALL_PLUS:
        buy(Goody::ExtraBallGoody, "goodyextraball");
        break;
    case CoinShopMenu::BUTTON_EXTRA_TIME_PLUS:
        buy(Goody::ExtraTimeGoody, "goodyextratime");
        break;
    case CoinShopMenu::BUTTON_GRAVITY_MINUS:
       {
            int refund = m_menu.getSellRefund(Goody::GravityGoody);
            int coins = m_config.get<int>("coins");
            int charge = m_config.get<int>("goodygravity");
            if(charge == -1)
            {
                charge = 0;
                m_menu.setGoodyCharges(Goody::GravityGoody, charge);
                m_config.set<int>("goodygravity", charge);
                m_config.set<int>("coins", coins + refund);
            }
        }
    case CoinShopMenu::BUTTON_INVULNERABLE_MINUS:
        sell(Goody::InvulnerableGoody, "goodyinvulnerable");
        break;
    case CoinShopMenu::BUTTON_EXTRA_BALL_MINUS:
        sell(Goody::ExtraBallGoody, "goodyextraball");
        break;
    case CoinShopMenu::BUTTON_EXTRA_TIME_MINUS:
        sell(Goody::ExtraTimeGoody, "goodyextratime");
        break;
    }
    return StateChangeInformation::Empty();
}

void CoinShopState::doDraw(const DrawParameter& params)
{
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    if(m_level != nullptr)
    {
        m_level->adjustView(params);
        m_HUD.update(m_level, getPassedTime());
        m_level->draw(params);
        m_HUD.draw(params);
        whiteRect.setFillColor(sf::Color(255, 255, 255, 128));
    }
    else
        whiteRect.setFillColor(sf::Color(255, 255, 255, 255));

    params.getTarget().draw(whiteRect);
    m_menu.draw(params);
}

void CoinShopState::updateButtons()
{
    int coins = m_config.get<int>("coins");
    m_menu.getButton(CoinShopMenu::BUTTON_GRAVITY_PLUS).setVisible(coins >= m_menu.getBuyCost(Goody::GravityGoody) && m_config.get<int>("goodygravity") >= 0);
    m_menu.getButton(CoinShopMenu::BUTTON_INVULNERABLE_PLUS).setVisible(coins >= m_menu.getBuyCost(Goody::InvulnerableGoody));
    m_menu.getButton(CoinShopMenu::BUTTON_EXTRA_BALL_PLUS).setVisible(coins >= m_menu.getBuyCost(Goody::ExtraBallGoody));
    m_menu.getButton(CoinShopMenu::BUTTON_EXTRA_TIME_PLUS).setVisible(coins >= m_menu.getBuyCost(Goody::ExtraTimeGoody));

    m_menu.getButton(CoinShopMenu::BUTTON_GRAVITY_MINUS).setVisible(m_config.get<int>("goodygravity") != 0);
    m_menu.getButton(CoinShopMenu::BUTTON_INVULNERABLE_MINUS).setVisible(m_config.get<int>("goodyinvulnerable") != 0);
    m_menu.getButton(CoinShopMenu::BUTTON_EXTRA_BALL_MINUS).setVisible(m_config.get<int>("goodyextraball") != 0);
    m_menu.getButton(CoinShopMenu::BUTTON_EXTRA_TIME_MINUS).setVisible(m_config.get<int>("goodyextratime") != 0);
}
