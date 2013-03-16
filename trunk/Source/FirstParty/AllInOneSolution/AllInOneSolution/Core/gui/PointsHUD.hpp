#pragma once

#ifndef POINTS_HPP
#define POINTS_HPP

#include "LabelHUD.hpp"
#include "animation\Storyboard.hpp"

class PointsHUD : public LabelHUD
{
public:
    PointsHUD(const sf::Vector2f& position,
              const float rotation,
              BitmapFont* bitmapFont,
		      const float horizontalPercentage = HUDElement::Left,
              const float verticalPercentage = HUDElement::Top);

    void update(const DrawParameter& params);

    void setPoints(int points);

    void setTime(float time);

private:
    Storyboard m_pointCounter;

    float m_elapsedTime;
};

#endif