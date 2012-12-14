#pragma once

#ifndef PARALLAX_LAYER_HPP
#define PARALLAX_LAYER_HPP

#include "AnimatedGraphics.hpp"

/// This is used to group animations into a specific layer which is scrolled parallax
class ParallaxLayer : public AnimatedGraphics
{
private:

    Animation* m_updatingAni;
    sf::Vector2f m_parallaxDistance;

public:

	ParallaxLayer(const sf::Vector2f& parallaxDistance);
	~ParallaxLayer();

    void update(const float time);

    void drawParallax(sf::RenderTarget& target, sf::RenderStates states, const sf::Vector2f& scrollPercent);

    virtual float getValueOf(const std::string& name) const;
};

#endif	//PARALLAX_LAYER_HPP

