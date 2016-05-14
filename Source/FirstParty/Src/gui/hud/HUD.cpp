#include "HUD.hpp"
#include "../../resources/AppConfig.hpp"
#include "../../model/Level.hpp"
#include "../../resources/ResourceManager.hpp"

#ifdef TOUCHSIM
#include "../../TouchController.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#endif

HUD::HUD(ResourceManager& resourceManager, AppConfig& config) :
    m_fpsCounter(sf::Vector2f(ScreenLocation::Left, ScreenLocation::Bottom), sf::Vector2f(30.f,-30.f), resourceManager.getBitmapFont("gold")),
    m_target(resourceManager, sf::Vector2f(ScreenLocation::Right, ScreenLocation::Top), sf::Vector2f(-30.f,10.f), resourceManager.getBitmapFont("gold")),
    m_points(resourceManager, sf::Vector2f(ScreenLocation::Left, ScreenLocation::Top), sf::Vector2f(40.f,10.f), resourceManager.getBitmapFont("gold")),
    m_arrow(resourceManager),
    m_ball(resourceManager, sf::Vector2f(0.66f, ScreenLocation::Top), sf::Vector2f(0.f,10.f), resourceManager.getBitmapFont("gold")),
    m_time(resourceManager, sf::Vector2f(0.33f, ScreenLocation::Top), sf::Vector2f(0.f,10.f), resourceManager.getBitmapFont("gold")),
    m_ballShow(false),
    m_timeShow(false),
    m_gravityGoody(resourceManager, "goodie", sf::IntRect(0,0,56,64), sf::Vector2f(ScreenLocation::Left, ScreenLocation::Top), sf::Vector2f(0,50)),
    m_invulnerableGoody(resourceManager, "goodie", sf::IntRect(56,0,56,64), sf::Vector2f(ScreenLocation::Left, ScreenLocation::Top), sf::Vector2f(0,114)),
    m_extraBallGoody(resourceManager, "goodie", sf::IntRect(112,0,56,64), sf::Vector2f(ScreenLocation::Left, ScreenLocation::Top), sf::Vector2f(0,178)),
    m_extraTimeGoody(resourceManager, "goodie", sf::IntRect(168,0,56,64), sf::Vector2f(ScreenLocation::Left, ScreenLocation::Top), sf::Vector2f(0,242))
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

    m_gravityGoody.updateState(level->getGoody(Goody::GravityGoody));
    m_invulnerableGoody.updateState(level->getGoody(Goody::InvulnerableGoody));
    m_extraTimeGoody.updateState(level->getGoody(Goody::ExtraTimeGoody));
    m_extraBallGoody.updateState(level->getGoody(Goody::ExtraBallGoody));
}

void HUD::restartAt(const float time)
{
    m_points.restartAt(time);
}

void HUD::draw(const DrawParameter& params)
{
    m_arrow.update(params);
    m_arrow.draw(params);

    params.getTarget().setView(utility::getDefaultView(params.getTarget(), params.getTarget().getView().getSize()));

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

    m_gravityGoody.update(params);
    m_gravityGoody.draw(params);

    m_invulnerableGoody.update(params);
    m_invulnerableGoody.draw(params);

    m_extraBallGoody.update(params);
    m_extraBallGoody.draw(params);

    m_extraTimeGoody.update(params);
    m_extraTimeGoody.draw(params);

#ifdef TOUCHSIM
    sf::RectangleShape teeterController(sf::Vector2f(utility::TeeterController.getTouchArea().width,
                                                     utility::TeeterController.getTouchArea().height));

    teeterController.setPosition(utility::TeeterController.getTouchArea().left,
                                 utility::TeeterController.getTouchArea().top);
    
    teeterController.setFillColor(sf::Color(255,255,255,128));
    teeterController.setOutlineColor(sf::Color(0,0,0,128));
    teeterController.setOutlineThickness(1.f);

    params.getTarget().draw(teeterController);
#endif
}

void HUD::setBallShow(bool ballShow)
{
    m_ballShow = ballShow;

    if(m_timeShow && m_ballShow)
    {
        m_time.setPosition(sf::Vector2f(0.33f, 0.f), sf::Vector2f(0.f,10.f));
        m_ball.setPosition(sf::Vector2f(0.66f, 0.f), sf::Vector2f(0.f,10.f));
    }
    else if(m_ballShow)
        m_ball.setPosition(sf::Vector2f(0.5f, 0.f), sf::Vector2f(0.f,10.f));
}

void HUD::setTimeShow(bool timeShow)
{
    m_timeShow = timeShow;

    if(m_timeShow && m_ballShow)
    {
        m_time.setPosition(sf::Vector2f(0.33f, 0.f), sf::Vector2f(0.f,10.f));
        m_ball.setPosition(sf::Vector2f(0.66f, 0.f), sf::Vector2f(0.f,10.f));
    }
    else if(m_timeShow)
        m_time.setPosition(sf::Vector2f(0.5f, 0.f), sf::Vector2f(0.f,10.f));
}

void HUD::onEnter(Level* level)
{
    m_timeShow = false;
    m_ballShow = false;
    m_points.setPoints(level != nullptr ? level->getPoints() : 0);
    m_points.skipInterpolation();
}
