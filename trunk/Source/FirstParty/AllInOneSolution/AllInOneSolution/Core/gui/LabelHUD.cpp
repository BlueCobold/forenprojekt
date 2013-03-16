#include "LabelHUD.hpp"
#include "../Utility.hpp"
#include "../resources/ResourceManager.hpp"

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
    HUDElement::update(params);
    m_label.setPosition(getPosition());
}

void LabelHUD::draw(const DrawParameter& params)
{    
    m_label.draw(params);
}

void LabelHUD::setText(const std::string& text)
{
    m_label.setText(text);
}
