#pragma once

#ifndef BALLHUD_HPP
#define BALLHUD_HPP

#include "LabelHUD.hpp"

/// 
class BallHUD : public LabelHUD
{
private:
    int m_remainingBall;

    sf::Sprite m_ballTexture;
    ResourceManager& m_resourceManager;

public:
    BallHUD(ResourceManager& resourceManager,
        const sf::Vector2f& position,
        const float rotation,
        BitmapFont* bitmapFont,
        float hReference = HUDElement::Left,
        float vReference = HUDElement::Top);

    void update(const DrawParameter& params);

    void draw(const DrawParameter& params);

    void setBalls(int remainingBall);
};

#endif
