#include "ParallaxLayer.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

ParallaxLayer::ParallaxLayer(const sf::Vector2f& layerSize) :
    m_layerSize(layerSize),
    m_updatingAni(nullptr)
{
}


ParallaxLayer::~ParallaxLayer()
{
}

void ParallaxLayer::updatePosition(const sf::View& view, const sf::Vector2u& worldSize)
{
    sf::Vector2f diff = sf::Vector2f(worldSize.x - view.getSize().x, 
        worldSize.y - view.getSize().y);
    sf::Vector2f viewPos = view.getCenter() - 0.5f * view.getSize();
    sf::Vector2f percent = sf::Vector2f(viewPos.x / diff.x, viewPos.y / diff.y);

    sf::Vector2f offset = viewPos + sf::Vector2f(
        -(m_layerSize.x - view.getSize().x) * percent.x,
        -(m_layerSize.y - view.getSize().y) * percent.y);
    
    updateAnimations([&](Animation& ani)->bool{
        ani.setPosition(offset.x, offset.y);
        return false;
    });
}

void ParallaxLayer::update(const double time)
{
    updateCurrentTime(time);
    updateAnimations([&](Animation& ani)->bool{
        m_updatingAni = &ani;
        return true;
    });
    m_updatingAni = nullptr;
}

double ParallaxLayer::onGetValueOf(const std::string& name) const
{
    if(m_updatingAni == nullptr)
        throw std::runtime_error(utility::replace(utility::translateKey("GetVariable"), name));
    return m_updatingAni->getValueOf(name);
}

void ParallaxLayer::onSetValueOf(const std::string& name, const double value)
{
    if(m_updatingAni == nullptr)
        throw std::runtime_error(utility::replace(utility::translateKey("SetVariable"), name));
    m_updatingAni->setValueOf(name, value);
}
