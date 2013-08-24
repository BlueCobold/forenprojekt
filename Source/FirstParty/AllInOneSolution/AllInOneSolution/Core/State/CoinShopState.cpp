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
    m_coinShopMenu(sf::Vector2f(0.0f, 0.0f), screen, resourceManager)
{
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

    int clicked = -1;
    m_coinShopMenu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_coinShopMenu.update(m_screen);

    if(clicked == CoinShopMenu::BUTTON_CLOSE)
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
    else if(clicked == CoinShopMenu::BUTTON_GRAVITY_PLUS)
    {
        int cost = m_coinShopMenu.getBuyCost(Goody::GravityGoody);
        int coins = m_config.get<int>("coins");
        int charge = m_config.get<int>("goodygravity");
        if(coins >= cost)
        {
            m_config.set<int>("goodygravity", ++charge);
            m_config.set<int>("coins", coins - cost);
        }
    }
    else if(clicked == CoinShopMenu::BUTTON_INVULNERABLE_PLUS)
    {
        int cost = m_coinShopMenu.getBuyCost(Goody::InvulnerableGoody);
        int coins = m_config.get<int>("coins");
        int charge = m_config.get<int>("goodyinvulnerable");
        if(coins >= cost)
        {
            m_config.set<int>("goodyinvulnerable", ++charge);
            m_config.set<int>("coins", coins - cost);
        }
    }
    else if(clicked == CoinShopMenu::BUTTON_EXTRA_BALL_PLUS)
    {
        int cost = m_coinShopMenu.getBuyCost(Goody::ExtraBallGoody);
        int coins = m_config.get<int>("coins");
        int charge = m_config.get<int>("goodyextraball");
        if(coins >= cost)
        {
            m_config.set<int>("goodyextraball", ++charge);
            m_config.set<int>("coins", coins - cost);
        }
    }
    else if(clicked == CoinShopMenu::BUTTON_EXTRA_TIME_PLUS)
    {
        int cost = m_coinShopMenu.getBuyCost(Goody::ExtraTimeGoody);
        int coins = m_config.get<int>("coins");
        int charge = m_config.get<int>("goodyextratime");
        if(coins >= cost)
        {
            m_config.set<int>("goodyextratime", ++charge);
            m_config.set<int>("coins", coins - cost);
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