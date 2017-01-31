#pragma once

#ifndef ANIMATIONCONTAINER_HPP
#define ANIMATIONCONTAINER_HPP

#include "../animation/AnimatedObject.hpp"
#include "../animation/Animation.hpp"
#include "../rendering/GraphicalObject.hpp"
#include "MenuElement.hpp"

class CloneHandler;

class AnimationContainer : public AnimatedObject, public MenuElement
{
private:
    std::map<std::string, double> m_variables;
    Animation* m_updatingAni;
    CloneHandler& m_cloneHandler;
    GraphicalObject graphics;
    sf::Vector2f m_scale;

    std::unique_ptr<MenuElement> doClone() const override;
    void onDrawElement(const DrawParameter& param) override;

    double onGetValueOf(const std::string& name) const override;
    void onSetValueOf(const std::string& name, const double value) override;
    void updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0)) override;

public:
    AnimationContainer(ScreenLocation position,
                       int id,
                       CloneHandler& cloneHandler);

    AnimationContainer(AnimationContainer&& toMove);

    void bindAnimation(std::unique_ptr<Animation> animation);

    void setScale(const sf::Vector2f& scale)
    {
        m_scale = scale;
    }
};

#endif