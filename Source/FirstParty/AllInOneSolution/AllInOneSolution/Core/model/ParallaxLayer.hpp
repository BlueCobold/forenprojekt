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

    void update(const float time, const sf::View& view, const sf::Vector2u& worldSize);

    virtual float getValueOf(const std::string& name) const;
};

#endif	//PARALLAX_LAYER_HPP

