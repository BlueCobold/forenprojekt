#pragma once

#ifndef POINTS_HPP
#define POINTS_HPP

#include "LabelHUD.hpp"
#include "../../animation/Interpolation.hpp"
#include "../../animation/TimedObject.hpp"
#include "../../resources/ResourceManager.hpp"

class PointsHUD : public LabelHUD, public TimedObject
{
public:
    PointsHUD(ResourceManager& resourceManager,
              const sf::Vector2f& position,
              const sf::Vector2f& offset,
              const BitmapFont* bitmapFont);

    void skipInterpolation();

    void update(const DrawParameter& params) override;

    void setPoints(int points);

    void setTime(float time);

private:

    void doDraw(const DrawParameter& params) override;
    void onRestarted() override;

    Interpolation m_pointCounter;

    ResourceManager& m_resourceManager;
    sf::Sprite m_coinTexture;
};

#endif
