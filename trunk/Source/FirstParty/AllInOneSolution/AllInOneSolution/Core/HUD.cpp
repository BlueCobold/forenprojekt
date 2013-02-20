#include "HUD.hpp"
#include "Config.hpp"
#include "model\Level.hpp"

HUD::HUD(sf::RenderTarget& screen, ResourceManager& resourceManager, Config& config) :
m_screen(screen),
m_resourceManager(resourceManager),
m_fpsCounter(resourceManager,sf::Vector2f(10.f,10.f),0.f,"gold"),
m_target(resourceManager,sf::Vector2f(900.f,10.f),0.f,"gold"),
m_points(resourceManager,sf::Vector2f(450.f,10.f),0.f,"gold")
{
    m_fpsShow = config.get<bool>("ShowFps");
}

void HUD::update(Level* level)
{
    m_fpsCounter.update(m_screen);

    m_target.setTargets(level->getTotalTarget(), level->getRemainingTarget());
    m_target.update(m_screen);

    m_points.setText(utility::toString<int>(level->getPoints()));
    m_points.update(m_screen);
}

void HUD::draw()
{
    m_target.draw(m_screen);

    m_points.draw(m_screen);

    if(m_fpsShow)
        m_fpsCounter.draw(m_screen);
}