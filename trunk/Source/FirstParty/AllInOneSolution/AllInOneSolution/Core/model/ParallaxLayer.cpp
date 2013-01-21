#include "ParallaxLayer.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
ParallaxLayer::ParallaxLayer(const sf::Vector2f& parallaxDistance) :
    m_parallaxDistance(parallaxDistance),
    m_updatingAni(nullptr)
{
}


ParallaxLayer::~ParallaxLayer()
{
}

void ParallaxLayer::update(const float time, const sf::Vector2f& scrollPercent)
{
    updateCurrentTime(time);
    
    sf::Vector2f offset = sf::Vector2f(
        m_parallaxDistance.x * (1 - scrollPercent.x * 2),
        m_parallaxDistance.y * (1 - scrollPercent.y * 2));

    for(auto animation = getAnimations().begin(); animation != getAnimations().end(); ++animation)
    {
        m_updatingAni = (*animation).get();
        (*animation)->setPosition(offset.x, offset.y);
        (*animation)->update();
    }
    m_updatingAni = nullptr;
}

float ParallaxLayer::getValueOf(const std::string& name) const
{
    if(m_updatingAni == nullptr)
        throw std::runtime_error("Can't evaluate a variable at this time.");
    return m_updatingAni->getValueOf(name);
}
