#include "HUDElement.hpp"
#include "Utility.hpp"
#include "resources\ResourceManager.hpp"

HUDElement::HUDElement(ResourceManager& resourceManager, const sf::Vector2f& position, const float rotation, std::string bitmapFont, std::string text) : 
m_resourceManager(resourceManager),
m_label(text,sf::Vector2f(0.f,0.f),rotation,nullptr),
m_bitmapfont(nullptr),
m_position(position)
{
    m_bitmapfont = m_resourceManager.getBitmapFont(bitmapFont);
    m_label.setBitmapFont(*m_bitmapfont);
}

void HUDElement::update(const DrawParameter& params)
{
    sf::Vector2f screenSize = params.getTarget().getView().getSize();
    sf::Vector2f screenCenter = params.getTarget().getView().getCenter();

    m_label.setPosition(screenCenter.x - screenSize.x / 2 + m_position.x , screenCenter.y - screenSize.y / 2 + m_position.y);
}

void HUDElement::draw(const DrawParameter& params)
{    
    m_label.draw(params);
}

void HUDElement::setPosition(sf::Vector2f position)
{
    m_position = position;
}

void HUDElement::setText(std::string text)
{
    m_label.setText(text);
}