#include "HUD.hpp"
#include "../../resources/Config.hpp"
#include "../../model/Level.hpp"

HUD::HUD(ResourceManager& resourceManager, Config& config) :
    m_fpsCounter(sf::Vector2f(30.f,10.f), 0.f, resourceManager.getBitmapFont("gold")),
    m_target(resourceManager, sf::Vector2f(-30.f,10.f), 0.f, resourceManager.getBitmapFont("gold"), HUDElement::Right),
    m_points(sf::Vector2f(0.f,10.f), 0.f, resourceManager.getBitmapFont("gold"), HUDElement::Center),
    m_arrow(resourceManager),
    m_ball(resourceManager, sf::Vector2f(0.f,10.f), 0.f, resourceManager.getBitmapFont("gold"), 0.66f),
    m_ballShow(false)
{
    m_fpsShow = config.get<bool>("ShowFps");
}

void HUD::update(const Level* level, const float elapsedTime)
{
    m_target.setTargets(level->getTotalTarget(), level->getRemainingTarget());

    m_points.setTime(elapsedTime);
    m_points.setPoints(level->getPoints());

    m_arrow.setBallCoords(level->getBallCoords());

    if(level->getRemainingBall() > 0 && !m_ballShow)
        setBallShow(true);

    m_ball.setBalls(level->getRemainingBall());
}

void HUD::restartAt(const float time)
{
    m_points.restartAt(time);
}

void HUD::draw(const DrawParameter& params)
{
    m_arrow.update(params);
    m_arrow.draw(params);

    m_target.update(params);
    m_target.draw(params);

    m_points.update(params);
    m_points.draw(params);

    m_ball.update(params);
    if(m_ballShow)
        m_ball.draw(params);

    m_fpsCounter.update(params);
    if(m_fpsShow)
        m_fpsCounter.draw(params);
}

void HUD::setBallShow(bool ballShow)
{
    m_ballShow = ballShow;

    if(m_ballShow)
        m_points.setPosition(sf::Vector2f(0.f,10.f), 0.33f);
    else
        m_points.setPosition(sf::Vector2f(0.f,10.f), HUDElement::Center);
}
