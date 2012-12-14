#pragma once

#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "AnimatedGraphics.hpp"
#include "ParallaxLayer.hpp"

#include <memory>
#include <vector>

/// This class will be used to bind an image which will be rendered as background
class Background : public AnimatedGraphics
{
private:

    Animation* m_updatingAni;
    std::vector<std::unique_ptr<ParallaxLayer>> m_layers;
    sf::Vector2u m_size;

public:

    Background(const sf::Vector2u& size);
	~Background();

    virtual void restartAt(const float time);
    void update(const float time);

    void bindLayer(std::unique_ptr<ParallaxLayer>& layer);

    virtual float getValueOf(const std::string& name) const;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif	//BACKGROUND_HPP

