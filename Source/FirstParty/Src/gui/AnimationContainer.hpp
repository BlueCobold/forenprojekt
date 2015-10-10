#pragma once

#ifndef ANIMATIONCONTAINER_HPP
#define ANIMATIONCONTAINER_HPP

#include "../animation/CloneHandler.hpp"
#include "../model/AnimatedGraphics.hpp"
#include "MenuElement.hpp"

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

    virtual std::unique_ptr<MenuElement> clone() const override;

    virtual float getValueOf(const std::string& name) const override;
    virtual void setValueOf(const std::string& name, const float value) override;
    virtual void update(const sf::RenderWindow& screen, const float time, const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0)) override;
    virtual void draw(const DrawParameter& param) override;
};

#endif