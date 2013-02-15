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

void Background::update(const float time, const sf::View& view)
{
    updateCurrentTime(time);

    m_updatingAni = nullptr;
    for(auto layer = m_layers.begin(); layer != m_layers.end(); ++layer)
        (*layer)->update(time, view, m_size);
}

float Background::getValueOf(const std::string& name) const
{
    if(m_updatingAni == nullptr)
        throw std::runtime_error("Can't evaluate a variable at this time.");
    return m_updatingAni->getValueOf(name);
}

void Background::setValueOf(const std::string& name, const float value)
{
    if(m_updatingAni == nullptr)
        throw std::runtime_error("Can't set a variable at this time.");
    return m_updatingAni->setValueOf(name, value);
}

void Background::draw(const DrawParameter& param)
{
    for(auto layer = m_layers.begin(); layer != m_layers.end(); ++layer)
        (*layer)->draw(param);
}
