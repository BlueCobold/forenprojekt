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
    m_icon.setTexture(*m_resourceManager.getTexture("GuiElements"));
    m_icon.setPosition(getCurrentPosition());
    setIcon();
}


void BatteryHUD::updated(const DrawParameter& params)
{
    m_icon.setPosition(getCurrentPosition());
    setIcon();
}

void BatteryHUD::doDraw(const DrawParameter& params)
{
    params.getTarget().draw(m_icon);
}

void BatteryHUD::setIcon()
{
    auto batteryState = utility::getBatteryState();
    
    if(batteryState.state == BatteryState::Charging)
        m_icon.setTextureRect(getIconRect("IconBatteryCharge"));
    else if(batteryState.percent > 0.75f)
        m_icon.setTextureRect(getIconRect("IconBattery100"));
    else if(batteryState.percent > 0.50f)
        m_icon.setTextureRect(getIconRect("IconBattery075"));
    else if(batteryState.percent > 0.25f)
        m_icon.setTextureRect(getIconRect("IconBattery050"));
    else if(batteryState.percent < 0.25f)
        m_icon.setTextureRect(getIconRect("IconBattery025"));
}

sf::IntRect BatteryHUD::getIconRect(const std::string& key)
{
    auto sheet = m_resourceManager.getSpriteSheet("gui_elements");
    auto data = sheet->get(key);

    return sf::IntRect(data.x, data.y, data.width, data.height);
}