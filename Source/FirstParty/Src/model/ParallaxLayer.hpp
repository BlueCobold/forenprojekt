#pragma once

#ifndef PARALLAX_LAYER_HPP
#define PARALLAX_LAYER_HPP

#include "AnimatedGraphics.hpp"

/// This is used to group animations into a specific layer which is scrolled parallax
class ParallaxLayer : public AnimatedGraphics
{
private:

    Animation* m_updatingAni;
    sf::Vector2f m_layerSize;

public:

    ParallaxLayer(const sf::Vector2f& layerSize);
    ~ParallaxLayer();

    void updatePosition(const sf::View& view, const sf::Vector2u& worldSize);
    void update(const float time);

    virtual float getValueOf(const std::string& name) const override;

    virtual void setValueOf(const std::string& name, const float value) override;
};

#endif //PARALLAX_LAYER_HPP
