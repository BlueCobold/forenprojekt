#include "LabelHUD.hpp"
#include "Utility.hpp"
#include "resources\ResourceManager.hpp"

LabelHUD::LabelHUD(const sf::Vector2f& position, 
                   const float rotation, 
                   BitmapFont* bitmapFont,
                   const float horizontalPercentage, 
                   const float verticalPercentage,
                   const std::string& text) :
    HUDElement(position, rotation, horizontalPercentage, verticalPercentage),
    m_label(text, sf::Vector2f(0.f, 0.f), rotation, nullptr)
{
    m_label.setBitmapFont(bitmapFont);
}

void LabelHUD::update(const DrawParameter& params)
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

void LabelHUD::draw(const DrawParameter& params)
{    
    m_label.draw(params);
}

void LabelHUD::setText(const std::string& text)
{
    m_label.setText(text);
}
