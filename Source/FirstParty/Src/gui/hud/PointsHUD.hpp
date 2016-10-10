#pragma once

#ifndef POINTS_HPP
#define POINTS_HPP

#include "LabelHUD.hpp"
#include "../../animation/Interpolation.hpp"
#include "../../resources/ResourceManager.hpp"

class PointsHUD : public LabelHUD
{
public:
    PointsHUD(ResourceManager& resourceManager,
              const sf::Vector2f& position,
              const sf::Vector2f& offset,
              const BitmapFont* bitmapFont);

    void skipInterpolation();

    void update(const DrawParameter& params) override;
    void restartAt(const float time);

    void setPoints(int points);

    void setTime(float time);

private:

    void doDraw(const DrawParameter& params) override;

    Interpolation m_pointCounter;

    float m_elapsedTime;

    ResourceManager& m_resourceManager;
    sf::Sprite m_coinTexture;
};

#endif
