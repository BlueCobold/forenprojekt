#pragma once

#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "AnimatedGraphics.hpp"
#include "ParallaxLayer.hpp"

#include <memory>
#include <vector>

/// This class will be used to bind an image which will be rendered as background
class Background : public Drawable, public TimedObject, public VariableHandler
{
private:

    Animation* m_updatingAni;
    std::vector<std::unique_ptr<ParallaxLayer>> m_layers;
    sf::Vector2u m_size;

public:

    Background(const sf::Vector2u& size);
    ~Background();

    virtual void restartAt(const float time) override;
    void update(const float time);
    void updateParallax(const sf::View& view);

    void bindLayer(std::unique_ptr<ParallaxLayer>&& layer);

    virtual float getValueOf(const std::string& name) const override;
    virtual void setValueOf(const std::string& name, const float value) override;

    virtual void draw(const DrawParameter& param) override;
};

#endif	//BACKGROUND_HPP

