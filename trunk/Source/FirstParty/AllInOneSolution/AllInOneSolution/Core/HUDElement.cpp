#include "HUDElement.hpp"
#include "Utility.hpp"
#include "resources\ResourceManager.hpp"

const float HUDElement::Left = 0.0f;
const float HUDElement::Right = 1.0f;
const float HUDElement::Center= 0.5f;
const float HUDElement::Top = 0.0f;
const float HUDElement::Middle = 0.5f;
const float HUDElement::Bottom = 1.0f;

HUDElement::HUDElement(const sf::Vector2f& position, 
                       const float rotation, 
                       BitmapFont* bitmapFont,
                       const float horizontalPercentage, 
                       const float verticalPercentage,
                       const std::string& text) :
    m_label(text, sf::Vector2f(0.f, 0.f), rotation, nullptr),
    m_position(position),
    m_verticalPercentage(verticalPercentage),
    m_horizontalPercentage(horizontalPercentage)
{
    m_label.setBitmapFont(bitmapFont);
}

void HUDElement::update(const DrawParameter& params)
{
    sf::Vector2f screenSize = params.getTarget().getView().getSize();
    sf::Vector2f screenCenter = params.getTarget().getView().getCenter();
    
    sf::Vector2f topLeftViewStart = screenCenter - screenSize * 0.5f;
    sf::Vector2f bottomRightViewEnd = screenCenter + screenSize * 0.5f;
    sf::Vector2f delta = bottomRightViewEnd - topLeftViewStart;

    sf::Vector2f position; 
    position.x =  topLeftViewStart.x + delta.x * m_horizontalPercentage + m_position.x;
    position.y =  topLeftViewStart.y + delta.y * m_verticalPercentage + m_position.y;

    m_label.setPosition(position);
}

void HUDElement::draw(const DrawParameter& params)
{    
    m_label.draw(params);
}

void HUDElement::setText(const std::string& text)
{
    m_label.setText(text);
}
