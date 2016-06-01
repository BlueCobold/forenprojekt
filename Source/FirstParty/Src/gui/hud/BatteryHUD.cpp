#include "BatteryHUD.hpp"
#include "../../resources/ResourceManager.hpp"
#include "../../Utility.hpp"
#include "../../resources/SpriteSheet.hpp"

BatteryHUD::BatteryHUD(ResourceManager& resourceManager,
                       const sf::Vector2f& position,
                       const sf::Vector2f& offset) :
    HUDElement(position, offset),
    m_resourceManager(resourceManager)
{
    m_icon.setTexture(*m_resourceManager.getTexture("battery"));
    m_icon.setPosition(getCurrentPosition());
    setIcon();
}


void BatteryHUD::update(const DrawParameter& params)
{
    HUDElement::update(params);

    m_icon.setPosition(getCurrentPosition());

    setIcon();
}

void BatteryHUD::draw(const DrawParameter& params)
{
    params.getTarget().draw(m_icon);
}

void BatteryHUD::setIcon()
{
    auto batteryState = utility::getBatteryState();
    
    if(batteryState.state == BatteryState::Charging)
        m_icon.setTextureRect(getIconRect("Charging"));
    else if(batteryState.percent > 0.75f)
        m_icon.setTextureRect(getIconRect("More75"));
    else if(batteryState.percent > 0.50f)
        m_icon.setTextureRect(getIconRect("More50"));
    else if(batteryState.percent > 0.25f)
        m_icon.setTextureRect(getIconRect("More25"));
    else if(batteryState.percent < 0.25f)
        m_icon.setTextureRect(getIconRect("Less25"));
}

sf::IntRect BatteryHUD::getIconRect(const std::string& key)
{
    auto sheet = m_resourceManager.getSpriteSheet("gui_battery");
    auto data = sheet->get(key);

    return sf::IntRect(data.x, data.y, data.width, data.height);
}