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

void Background::update(const float time, const sf::View& view)
{
    updateCurrentTime(time);

    m_updatingAni = nullptr;
    for(auto layer = begin(m_layers); layer != end(m_layers); ++layer)
        (*layer)->update(time, view, m_size);
}

float Background::getValueOf(const std::string& name) const
{
    if(m_updatingAni == nullptr)
         throw std::exception(utility::replace(utility::translateKey("GetVariable"), name).c_str());
    return m_updatingAni->getValueOf(name);
}

void Background::setValueOf(const std::string& name, const float value)
{
    if(m_updatingAni == nullptr)
        throw std::exception(utility::replace(utility::translateKey("SetVariable"), name).c_str());
    m_updatingAni->setValueOf(name, value);
}

void Background::draw(const DrawParameter& param)
{
    for(auto layer = begin(m_layers); layer != end(m_layers); ++layer)
        (*layer)->draw(param);
}
