#include "HUD.hpp"
#include "resources/Config.hpp"
#include "model/Level.hpp"

HUD::HUD(ResourceManager& resourceManager, Config& config) :
    m_fpsCounter(sf::Vector2f(30.f,10.f), 0.f, resourceManager.getBitmapFont("gold")),
    m_target(resourceManager, sf::Vector2f(-30.f,10.f), 0.f, resourceManager.getBitmapFont("gold"), HUDElement::Right),
    m_points(sf::Vector2f(0.f,10.f), 0.f, resourceManager.getBitmapFont("gold"), HUDElement::Center),
    m_arrow(resourceManager)
{
    m_fpsShow = config.get<bool>("ShowFps");
}

void HUD::update(const Level* level, float elapsedTime)
{
    m_target.setTargets(level->getTotalTarget(), level->getRemainingTarget());

    m_points.setTime(elapsedTime);
    m_points.setPoints(level->getPoints());

    m_arrow.setBallCoords(level->getBallCoords());
}

void HUD::draw(const DrawParameter& params)
{
    m_arrow.update(params);
    m_arrow.draw(params);

    m_target.update(params);
    m_target.draw(params);

    m_points.update(params);
    m_points.draw(params);

    m_fpsCounter.update(params);
    if(m_fpsShow)
        m_fpsCounter.draw(params);
}
