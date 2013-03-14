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

void ParallaxLayer::update(const float time, const sf::View& view, const sf::Vector2u& worldSize)
{
    updateCurrentTime(time);
    
    sf::Vector2f diff = sf::Vector2f(worldSize.x - view.getSize().x, 
                                     worldSize.y - view.getSize().y);
    sf::Vector2f viewPos = view.getCenter() - 0.5f * view.getSize();
    sf::Vector2f percent = sf::Vector2f(viewPos.x / diff.x, viewPos.y / diff.y);

    sf::Vector2f offset = viewPos + sf::Vector2f(
        -(m_layerSize.x - view.getSize().x) * percent.x,
        -(m_layerSize.y - view.getSize().y) * percent.y);

    for(auto animation = begin(getAnimations()); animation != end(getAnimations()); ++animation)
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
        throw std::exception(utility::replace(utility::translateKey("GetVariable"), name).c_str());
    return m_updatingAni->getValueOf(name);
}

void ParallaxLayer::setValueOf(const std::string& name, const float value)
{
    if(m_updatingAni == nullptr)
        throw std::exception(utility::replace(utility::translateKey("SetVariable"), name).c_str());
    m_updatingAni->setValueOf(name, value);
}