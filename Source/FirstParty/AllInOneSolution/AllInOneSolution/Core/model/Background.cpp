#include "Background.hpp"
#include <SFML/Graphics/RenderTarget.hpp>


Background::Background(const sf::Vector2u& size) : m_size(size), m_updatingAni(nullptr)
{
}


Background::~Background()
{
}

void Background::bindLayer(std::unique_ptr<ParallaxLayer>& layer)
{
    m_layers.push_back(std::move(layer));
}

void Background::restartAt(const float time)
{
    TimedObject::restartAt(time);
    for(auto layer = m_layers.begin(); layer != m_layers.end(); ++layer)
        (*layer)->restartAt(time);
}

void Background::update(const float time)
{
    updateCurrentTime(time);
    for(auto animation = getAnimations().begin(); animation != getAnimations().end(); ++animation)
    {
        m_updatingAni = (*animation).get();
        (*animation)->update();
    }
    m_updatingAni = nullptr;
    for(auto layer = m_layers.begin(); layer != m_layers.end(); ++layer)
        (*layer)->update(time);
}

float Background::getValueOf(const std::string& name) const
{
    if(m_updatingAni == nullptr)
        throw std::runtime_error("Can't evaluate a variable at this time.");
    return m_updatingAni->getValueOf(name);
}

void Background::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    GraphicalObject::draw(target, states);

    sf::Vector2f scroll = sf::Vector2f(0.5f, 0.5f);
    float xper = (m_size.x-target.getView().getSize().x);
    if (fabs(xper) > FLT_EPSILON)
        scroll.x = (target.getView().getCenter().x - 0.5f*target.getView().getSize().x) / xper;
    float yper = (m_size.y-target.getView().getSize().y);
    if (fabs(yper)>FLT_EPSILON)
        scroll.y = (target.getView().getCenter().y - 0.5f*target.getView().getSize().y) / yper;
    for(auto layer = m_layers.begin(); layer != m_layers.end(); ++layer)
        (*layer)->drawParallax(target, states, scroll);
}
