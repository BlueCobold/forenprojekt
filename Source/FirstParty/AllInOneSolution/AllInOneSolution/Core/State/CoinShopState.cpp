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
    m_clicked(-1)
{
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_GRAVITY_PLUS).registerOnPressed([&](const Button& sender){ m_clicked = sender.getId(); });
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_INVULNERABLE_PLUS).registerOnPressed([&](const Button& sender){ m_clicked = sender.getId(); });
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_EXTRA_TIME_PLUS).registerOnPressed([&](const Button& sender){ m_clicked = sender.getId(); });
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_EXTRA_BALL_PLUS).registerOnPressed([&](const Button& sender){ m_clicked = sender.getId(); });
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_GRAVITY_MINUS).registerOnPressed([&](const Button& sender){ m_clicked = sender.getId(); });
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_INVULNERABLE_MINUS).registerOnPressed([&](const Button& sender){ m_clicked = sender.getId(); });
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_EXTRA_TIME_MINUS).registerOnPressed([&](const Button& sender){ m_clicked = sender.getId(); });
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_EXTRA_BALL_MINUS).registerOnPressed([&](const Button& sender){ m_clicked = sender.getId(); });
    m_coinShopMenu.getButton(CoinShopMenu::BUTTON_CLOSE).registerOnPressed([&](const Button& sender){ m_clicked = sender.getId(); });
}

CoinShopState::~CoinShopState()
{
}

void CoinShopState::onEnter(const EnterStateInformation* enterInformation, const float time)
{
    m_level = enterInformation->m_level;
    m_levelNumber = enterInformation->m_levelNumber;

    m_coinShopMenu.setGoodyCharges(Goody::GravityGoody, m_config.get<int>("goodygravity"));
    m_coinShopMenu.setGoodyCharges(Goody::InvulnerableGoody, m_config.get<int>("goodyinvulnerable"));
    m_coinShopMenu.setGoodyCharges(Goody::ExtraBallGoody, m_config.get<int>("goodyextraball"));
    m_coinShopMenu.setGoodyCharges(Goody::ExtraTimeGoody, m_config.get<int>("goodyextratime"));
}

StateChangeInformation CoinShopState::update(const float time)
{
    m_coinShopMenu.setPosition(sf::Vector2f(m_screen.getSize().x / 2.f - m_coinShopMenu.getSize().x / 2.f, m_screen.getSize().y / 2.f - m_coinShopMenu.getSize().y / 2.f));

    updateTime(time);

    m_clicked = -1;
    m_coinShopMenu.update(m_screen);

    if(m_clicked == CoinShopMenu::BUTTON_CLOSE)
    {
        m_levelPreviewInfo.m_level = m_level;
        m_levelPreviewInfo.m_levelNumber = m_levelNumber;
        m_levelPreviewInfo.m_prepareOnly = false;
        m_levelPreviewInfo.m_returnFromPause = false;
        m_transitionStateInfo.m_level = m_level;
        m_transitionStateInfo.m_followingState = LevelPreviewStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_levelPreviewInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(m_clicked == CoinShopMenu::BUTTON_GRAVITY_PLUS)
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
    else if(m_clicked == CoinShopMenu::BUTTON_INVULNERABLE_PLUS)
    {
        int cost = m_coinShopMenu.getBuyCost(Goody::InvulnerableGoody);
        int coins = m_config.get<int>("coins");
        int charge = m_config.get<int>("goodyinvulnerable");
        if(coins >= cost)
        {
            charge++;
            m_coinShopMenu.setGoodyCharges(Goody::InvulnerableGoody, charge);
            m_config.set<int>("goodyinvulnerable", charge);
            m_config.set<int>("coins", coins - cost);
        }
    }
    else if(m_clicked == CoinShopMenu::BUTTON_EXTRA_BALL_PLUS)
    {
        int cost = m_coinShopMenu.getBuyCost(Goody::ExtraBallGoody);
        int coins = m_config.get<int>("coins");
        int charge = m_config.get<int>("goodyextraball");
        if(coins >= cost)
        {
            charge++;
            m_coinShopMenu.setGoodyCharges(Goody::ExtraBallGoody, charge);
            m_config.set<int>("goodyextraball", charge);
            m_config.set<int>("coins", coins - cost);
        }
    }
    else if(m_clicked == CoinShopMenu::BUTTON_EXTRA_TIME_PLUS)
    {
        int cost = m_coinShopMenu.getBuyCost(Goody::ExtraTimeGoody);
        int coins = m_config.get<int>("coins");
        int charge = m_config.get<int>("goodyextratime");
        if(coins >= cost)
        {
            charge++;
            m_coinShopMenu.setGoodyCharges(Goody::ExtraTimeGoody, charge);
            m_config.set<int>("goodyextratime", charge);
            m_config.set<int>("coins", coins - cost);
        }
    }
    else if(m_clicked == CoinShopMenu::BUTTON_GRAVITY_MINUS)
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
    else if(m_clicked == CoinShopMenu::BUTTON_INVULNERABLE_MINUS)
    {
        int refund = m_coinShopMenu.getSellRefund(Goody::InvulnerableGoody);
        int coins = m_config.get<int>("coins");
        int charge = m_config.get<int>("goodyinvulnerable");
        if(charge != 0)
        {
            charge--;
            m_coinShopMenu.setGoodyCharges(Goody::InvulnerableGoody, charge);
            m_config.set<int>("goodyinvulnerable", charge);
            m_config.set<int>("coins", coins + refund);
        }
    }
    else if(m_clicked == CoinShopMenu::BUTTON_EXTRA_BALL_MINUS)
    {
        int refund = m_coinShopMenu.getSellRefund(Goody::ExtraBallGoody);
        int coins = m_config.get<int>("coins");
        int charge = m_config.get<int>("goodyextraball");
        if(charge != 0)
        {
            charge--;
            m_coinShopMenu.setGoodyCharges(Goody::ExtraBallGoody, charge);
            m_config.set<int>("goodyextraball", charge);
            m_config.set<int>("coins", coins + refund);
        }
    }
    else if(m_clicked == CoinShopMenu::BUTTON_EXTRA_TIME_MINUS)
    {
        int refund = m_coinShopMenu.getSellRefund(Goody::ExtraTimeGoody);
        int coins = m_config.get<int>("coins");
        int charge = m_config.get<int>("goodyextratime");
        if(charge != 0)
        {
            charge--;
            m_coinShopMenu.setGoodyCharges(Goody::ExtraTimeGoody, charge);
            m_config.set<int>("goodyextratime", charge);
            m_config.set<int>("coins", coins + refund);
        }
    }
    return StateChangeInformation::Empty();
}

void CoinShopState::draw(const DrawParameter& params)
{
    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 255));
    params.getTarget().draw(whiteRect);

    m_coinShopMenu.draw(params);
}
