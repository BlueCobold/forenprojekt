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
    for(auto layer = begin(m_layers); layer != end(m_layers); ++layer)
        (*layer)->restartAt(time);
}

void Background::update(const float time)
{
    updateCurrentTime(time);

    m_updatingAni = nullptr;
    for(auto layer = begin(m_layers); layer != end(m_layers); ++layer)
        (*layer)->update(time);
}

void Background::updateParallax(const sf::View& view)
{
    for(auto layer = begin(m_layers); layer != end(m_layers); ++layer)
        (*layer)->updatePosition(view, m_size);
}

float Background::getValueOf(const std::string& name) const
{
    if(m_updatingAni == nullptr)
         throw std::runtime_error(utility::replace(utility::translateKey("GetVariable"), name));
    return m_updatingAni->getValueOf(name);
}

void Background::setValueOf(const std::string& name, const float value)
{
    if(m_updatingAni == nullptr)
        throw std::runtime_error(utility::replace(utility::translateKey("SetVariable"), name));
    m_updatingAni->setValueOf(name, value);
}

void Background::draw(const DrawParameter& param)
{
    for(auto layer = begin(m_layers); layer != end(m_layers); ++layer)
        (*layer)->draw(param);
}
