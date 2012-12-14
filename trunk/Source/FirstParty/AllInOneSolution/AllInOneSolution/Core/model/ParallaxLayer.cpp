#include "ParallaxLayer.hpp"
#include <SFML/Graphics/RenderTarget.hpp>


ParallaxLayer::ParallaxLayer(const sf::Vector2f& parallaxDistance) :
    m_parallaxDistance(parallaxDistance),
    m_updatingAni(nullptr)
{
}


ParallaxLayer::~ParallaxLayer()
{
}

void ParallaxLayer::update(const float time)
{
    updateCurrentTime(time);
    for(auto animation = getAnimations().begin(); animation != getAnimations().end(); ++animation)
    {
        m_updatingAni = (*animation).get();
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

void ParallaxLayer::drawParallax(sf::RenderTarget& target, sf::RenderStates states, const sf::Vector2f& scrollPercent)
{
    sf::Vector2f offset = sf::Vector2f(
        (1 - m_parallaxDistance.x * 2) * scrollPercent.x,
        (1 - m_parallaxDistance.y * 2) * scrollPercent.y);
    for(auto animation = getAnimations().begin(); animation != getAnimations().end(); ++animation)
        (*animation)->setPosition(offset.x, offset.y);
    draw(target, states);
}
