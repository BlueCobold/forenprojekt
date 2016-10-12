#pragma once

#ifndef PARALLAX_LAYER_HPP
#define PARALLAX_LAYER_HPP

#include "AnimatedGraphics.hpp"

/// This is used to group animations into a specific layer which is scrolled parallax
class ParallaxLayer : public AnimatedGraphics
{
private:

    sf::Vector2f m_layerSize;
    Animation* m_updatingAni;

    float onGetValueOf(const std::string& name) const override;
    void onSetValueOf(const std::string& name, const float value) override;

public:

    ParallaxLayer(const sf::Vector2f& layerSize);
    ~ParallaxLayer();

    void updatePosition(const sf::View& view, const sf::Vector2u& worldSize);
    void update(const float time);
};

#endif //PARALLAX_LAYER_HPP
