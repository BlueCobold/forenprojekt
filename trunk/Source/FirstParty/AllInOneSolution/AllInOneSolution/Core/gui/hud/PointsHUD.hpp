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
              const float rotation,
              BitmapFont* bitmapFont,
              const float horizontalPercentage = HUDElement::Left,
              const float verticalPercentage = HUDElement::Top);

    void skipInterpolation();

    void update(const DrawParameter& params);
    void restartAt(const float time);

    void setPoints(int points);

    void setTime(float time);

    void draw(const DrawParameter& params) override;

private:
    Interpolation m_pointCounter;

    float m_elapsedTime;

    ResourceManager& m_resourceManager;
    sf::Sprite m_coinTexture;
};

#endif
