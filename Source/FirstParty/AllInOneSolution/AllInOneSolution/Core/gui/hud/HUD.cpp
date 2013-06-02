#include "HUD.hpp"
#include "../../resources/Config.hpp"
#include "../../model/Level.hpp"

HUD::HUD(ResourceManager& resourceManager, Config& config) :
    m_fpsCounter(sf::Vector2f(30.f,-30.f), 0.f, resourceManager.getBitmapFont("gold"), HUDElement::Left, HUDElement::Bottom),
    m_target(resourceManager, sf::Vector2f(-30.f,10.f), 0.f, resourceManager.getBitmapFont("gold"), HUDElement::Right),
    m_points(sf::Vector2f(30.f,10.f), 0.f, resourceManager.getBitmapFont("gold"), HUDElement::Left),
    m_arrow(resourceManager),
    m_ball(resourceManager, sf::Vector2f(0.f,10.f), 0.f, resourceManager.getBitmapFont("gold"), 0.66f),
    m_time(resourceManager, sf::Vector2f(0.f,10.f), 0.f, resourceManager.getBitmapFont("gold"), 0.33f),
    m_ballShow(false),
    m_timeShow(false)
{
    m_fpsShow = config.get<bool>("ShowFps");
}

void HUD::skipInterpolation()
{
    m_points.skipInterpolation();
}

void HUD::update(const Level* level, const float elapsedTime)
{
    m_target.setTargets(level->getTotalTarget(), level->getRemainingTarget());

    m_points.setTime(elapsedTime);
    m_points.setPoints(level->getPoints());

    m_arrow.setBallCoords(level->getBallCoords());

    if(level->getRemainingBall() > 0 && !m_ballShow)
        setBallShow(true);

    if(level->getRemainigTime() > 0 && !m_timeShow)
        setTimeShow(true);

    m_ball.setBalls(level->getRemainingBall());

    m_time.setTime(static_cast<int>(level->getRemainigTime() + 1));
}

void HUD::restartAt(const float time)
{
    m_points.restartAt(time);
}

void HUD::draw(const DrawParameter& params)
{
    m_arrow.update(params);
    m_arrow.draw(params);

    params.getTarget().setView(utility::getDefaultView(params.getTarget(), params.getTarget().getSize()));
   
    m_target.update(params);
    m_target.draw(params);

    m_points.update(params);
    m_points.draw(params);

    m_ball.update(params);
    if(m_ballShow)
        m_ball.draw(params);

    m_time.update(params);
    if(m_timeShow)
        m_time.draw(params);

    m_fpsCounter.update(params);
    if(m_fpsShow)
        m_fpsCounter.draw(params);
}

void HUD::setBallShow(bool ballShow)
{
    m_ballShow = ballShow;

    if(m_timeShow && m_ballShow)
    {
        m_time.setPosition(sf::Vector2f(0.f,10.f), 0.33f);
        m_ball.setPosition(sf::Vector2f(0.f,10.f), 0.66f);
    }
    else if(m_ballShow)
        m_ball.setPosition(sf::Vector2f(0.f,10.f), 0.5f);
}

void HUD::setTimeShow(bool timeShow)
{
    m_timeShow = timeShow;

    if(m_timeShow && m_ballShow)
    {
        m_time.setPosition(sf::Vector2f(0.f,10.f), 0.33f);
        m_ball.setPosition(sf::Vector2f(0.f,10.f), 0.66f);
    }
    else if(m_timeShow)
        m_time.setPosition(sf::Vector2f(0.f,10.f), 0.5f);
}