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

    sf::Vector2u m_size;
    Animation* m_updatingAni;
    std::vector<std::unique_ptr<ParallaxLayer>> m_layers;

    void doDraw(const DrawParameter& param) override;
    void onRestarted() override;

    float onGetValueOf(const std::string& name) const override;
    void onSetValueOf(const std::string& name, const float value) override;

public:

    Background(const sf::Vector2u& size);
    ~Background();

    void update(const float time);
    void updateParallax(const sf::View& view);

    void bindLayer(std::unique_ptr<ParallaxLayer>&& layer);
};

#endif //BACKGROUND_HPP
