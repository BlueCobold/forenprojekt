#pragma once

#ifndef BALLHUD_HPP
#define BALLHUD_HPP

#include "LabelHUD.hpp"

/// 
class BallHUD : public LabelHUD
{
private:
    int m_remainingBall;

    sf::Sprite m_ball;
    ResourceManager& m_resourceManager;

public:
    BallHUD(ResourceManager& resourceManager,
            const sf::Vector2f& position,
            const sf::Vector2f& offset,
            const BitmapFont* bitmapFont);

    void update(const DrawParameter& params);

    void doDraw(const DrawParameter& params) override;

    void setBalls(int remainingBall);
};

#endif
