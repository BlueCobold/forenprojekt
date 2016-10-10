#pragma once

#ifndef BATTERYHUD_HPP
#define BATTERYHUD_HPP

#include "HUDElement.hpp"
#include <SFML/Graphics/Sprite.hpp>

class BatteryHUD : public HUDElement
{
private:
    sf::Sprite m_icon;

    ResourceManager& m_resourceManager;

    sf::IntRect getIconRect(const std::string& key);

    void setIcon();

public:
    BatteryHUD(ResourceManager& resourceManager,
               const sf::Vector2f& position,
               const sf::Vector2f& offset);

    void update(const DrawParameter& params);

    void doDraw(const DrawParameter& params) override;
};

#endif