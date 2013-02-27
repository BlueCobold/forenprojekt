#include "HUD.hpp"
#include "Config.hpp"
#include "model\Level.hpp"

HUD::HUD(ResourceManager& resourceManager, Config& config) :
	m_resourceManager(resourceManager),
	m_fpsCounter(resourceManager,sf::Vector2f(50.f,10.f),0.f,"gold"),
	m_target(resourceManager, sf::Vector2f(60.f,10.f),0.f, "gold", HUDElement::Right),
	m_points(resourceManager, sf::Vector2f(0.f,10.f),0.f, "gold", HUDElement::Center)
{
    m_fpsShow = config.get<bool>("ShowFps");
}
void HUD::update(Level* level)
{

    m_target.setTargets(level->getTotalTarget(), level->getRemainingTarget());

    m_points.setText(utility::toString<int>(level->getPoints()));
}

void HUD::draw(const DrawParameter& params)
{
    m_target.update(params);
    m_target.draw(params);

    m_points.update(params);
    m_points.draw(params);

    m_fpsCounter.update(params);
    if(m_fpsShow)
        m_fpsCounter.draw(params);
}