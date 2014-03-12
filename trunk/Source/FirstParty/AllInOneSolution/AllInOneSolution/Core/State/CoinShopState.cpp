#include "CoinShopState.hpp"

#include "../gui/Button.hpp"
#include "../Input.hpp"
#include "../resources/Config.hpp"
#include "../resources/ResourceManager.hpp"
#include "../rendering/transitions/RandomTransition.hpp"
#include "../model/Level.hpp"
#include "EnterStateInformation.hpp"
#include "../resources/AppConfig.hpp"

#include <memory>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>

CoinShopState::CoinShopState(sf::RenderWindow& screen,
                             ResourceManager& resourceManager,
                             AppConfig& config) :
    State(screen, resourceManager, config),
    m_coinShopMenu(sf::Vector2f(0.0f, 0.0f), screen, resourceManager),
    m_HUD(resourceManager, config),
    m_clicked(-1)
{
    auto buttonFunc = [&](const Button& sender){ m_clicked = sender.getId(); };
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_GRAVITY_PLUS).registerOnPressed(buttonFunc);
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_INVULNERABLE_PLUS).registerOnPressed(buttonFunc);
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_EXTRA_TIME_PLUS).registerOnPressed(buttonFunc);
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_EXTRA_BALL_PLUS).registerOnPressed(buttonFunc);
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_GRAVITY_MINUS).registerOnPressed(buttonFunc);
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_INVULNERABLE_MINUS).registerOnPressed(buttonFunc);
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_EXTRA_TIME_MINUS).registerOnPressed(buttonFunc);
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_EXTRA_BALL_MINUS).registerOnPressed(buttonFunc);
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_CLOSE).registerOnPressed(buttonFunc);
}

CoinShopState::~CoinShopState()
{
}

void CoinShopState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    State::onEnter(enterInformation, time);

    m_level = enterInformation->m_level;
    m_levelNumber = enterInformation->m_levelNumber;

    m_coinShopMenu.setGoodyCharges(Goody::GravityGoody, m_config.get<int>("goodygravity"));
    m_coinShopMenu.setGoodyCharges(Goody::InvulnerableGoody, m_config.get<int>("goodyinvulnerable"));
    m_coinShopMenu.setGoodyCharges(Goody::ExtraBallGoody, m_config.get<int>("goodyextraball"));
    m_coinShopMenu.setGoodyCharges(Goody::ExtraTimeGoody, m_config.get<int>("goodyextratime"));
}

void CoinShopState::buy(const Goody::Type type, const std::string& propertyKey)
{
    int cost = m_coinShopMenu.getBuyCost(type);
    int coins = m_config.get<int>("coins");
    int charge = m_config.get<int>(propertyKey);
    if(coins >= cost)
    {
        charge++;
        m_coinShopMenu.setGoodyCharges(type, charge);
        m_config.set<int>(propertyKey, charge);
        m_config.set<int>("coins", coins - cost);
    }
}

void CoinShopState::sell(const Goody::Type type, const std::string& propertyKey)
{
    int refund = m_coinShopMenu.getSellRefund(type);
    int coins = m_config.get<int>("coins");
    int charge = m_config.get<int>(propertyKey);
    if(charge != 0)
    {
        charge--;
        m_coinShopMenu.setGoodyCharges(type, charge);
        m_config.set<int>(propertyKey, charge);
        m_config.set<int>("coins", coins + refund);
    }
}

StateChangeInformation CoinShopState::update(const float time)
{
    if(State::isPaused())
        return StateChangeInformation::Empty();

    m_coinShopMenu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_coinShopMenu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_coinShopMenu.getSize().y / 2.f));

    updateTime(time);

    m_clicked = -1;
    m_coinShopMenu.update(m_screen, getPassedTime());
    updateButtons();

    switch(m_clicked)
    {
    case CoinShopMenu::BUTTON_CLOSE:
        m_levelPreviewInfo.m_level = m_level;
        m_levelPreviewInfo.m_levelNumber = m_levelNumber;
        m_levelPreviewInfo.m_prepareOnly = false;
        m_levelPreviewInfo.m_returnFromPause = false;
        m_transitionStateInfo.m_level = m_level;
        m_transitionStateInfo.m_followingState = LevelPreviewStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_levelPreviewInfo;
        m_transitionStateInfo.m_comeFromeState = CoinShopStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    case CoinShopMenu::BUTTON_GRAVITY_PLUS:
        {
            int cost = m_coinShopMenu.getBuyCost(Goody::GravityGoody);
            int coins = m_config.get<int>("coins");
            int charge = m_config.get<int>("goodygravity");
            if(coins >= cost && charge == 0)
            {
                charge = -1;
                m_coinShopMenu.setGoodyCharges(Goody::GravityGoody, charge);
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
            int refund = m_coinShopMenu.getSellRefund(Goody::GravityGoody);
            int coins = m_config.get<int>("coins");
            int charge = m_config.get<int>("goodygravity");
            if(charge == -1)
            {
                charge = 0;
                m_coinShopMenu.setGoodyCharges(Goody::GravityGoody, charge);
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

void CoinShopState::draw(const DrawParameter& params)
{
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    if(m_level != nullptr)
    {
        m_level->adjustView(params.getTarget());
        m_HUD.update(m_level, getPassedTime());
        m_level->draw(params);
        m_HUD.draw(params);
        whiteRect.setFillColor(sf::Color(255, 255, 255, 128));
    }
    else
        whiteRect.setFillColor(sf::Color(255, 255, 255, 255));
    params.getTarget().draw(whiteRect);

    m_coinShopMenu.draw(params);
    /*params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 255));
    params.getTarget().draw(whiteRect);

    m_coinShopMenu.draw(params);*/
}

void CoinShopState::updateButtons()
{
    int coins = m_config.get<int>("coins");
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_GRAVITY_PLUS).setVisible(coins >= m_coinShopMenu.getBuyCost(Goody::GravityGoody) && m_config.get<int>("goodygravity") >= 0);
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_INVULNERABLE_PLUS).setVisible(coins >= m_coinShopMenu.getBuyCost(Goody::InvulnerableGoody));
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_EXTRA_BALL_PLUS).setVisible(coins >= m_coinShopMenu.getBuyCost(Goody::ExtraBallGoody));
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_EXTRA_TIME_PLUS).setVisible(coins >= m_coinShopMenu.getBuyCost(Goody::ExtraTimeGoody));

    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_GRAVITY_MINUS).setVisible(m_config.get<int>("goodygravity") != 0);
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_INVULNERABLE_MINUS).setVisible(m_config.get<int>("goodyinvulnerable") != 0);
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_EXTRA_BALL_MINUS).setVisible(m_config.get<int>("goodyextraball") != 0);
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_EXTRA_TIME_MINUS).setVisible(m_config.get<int>("goodyextratime") != 0);
}
