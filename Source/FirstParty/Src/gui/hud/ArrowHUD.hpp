#pragma once

#ifndef ARROW_HPP
#define ARROW_HPP

#include "HUDElement.hpp"
#include <SFML/Graphics/Sprite.hpp>

class ArrowHUD : public HUDElement
{
    static const int ARROW_WIDTH = 64;

    sf::Sprite m_arrow;
    ResourceManager& m_resourceManager;
    sf::Vector2f m_ballPos;
    bool m_outOfView;

    void updated(const DrawParameter& params) override;
    void doDraw(const DrawParameter& params) override;

public:
    ArrowHUD(ResourceManager& resourceManager);

    void setBallCoords(const sf::Vector2f& ballPos);
};

#endif /// ARROW_HPP
