#pragma once

#ifndef ANIMATIONCONTAINER_HPP
#define ANIMATIONCONTAINER_HPP

#include "../model/AnimatedGraphics.hpp"
#include "MenuElement.hpp"

class CloneHandler;

class AnimationContainer : public virtual AnimatedGraphics, public virtual MenuElement
{
private:
    std::map<std::string, float> m_variables;
    Animation* m_updatingAni;
    CloneHandler& m_cloneHandler;

public:
    AnimationContainer(const sf::Vector2f& position, 
                       const sf::Vector2f& offset,
                       int id,
                       CloneHandler& cloneHandler);
    AnimationContainer(AnimationContainer&& toMove);

    std::unique_ptr<MenuElement> clone() const override;

    float getValueOf(const std::string& name) const override;
    void setValueOf(const std::string& name, const float value) override;
    void update(const sf::RenderWindow& screen, const float time, const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0)) override;
    void draw(const DrawParameter& param) override;
    void updateLayout(const sf::Vector2f& screenSize) override;
};

#endif