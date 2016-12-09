#pragma once

#ifndef BATTERYHUD_HPP
#define BATTERYHUD_HPP

#include "HUDElement.hpp"
#include "../../rendering/Sprite.hpp"

class BatteryHUD : public HUDElement
{
    Sprite m_icon;
    ResourceManager& m_resourceManager;

    sf::IntRect getIconRect(const std::string& key);
    void setIcon();
    
    void doDraw(const DrawParameter& params) override;
    void updated(const DrawParameter& params) override;

public:
    BatteryHUD(ResourceManager& resourceManager,
               const sf::Vector2f& position,
               const sf::Vector2f& offset);
};

#endif