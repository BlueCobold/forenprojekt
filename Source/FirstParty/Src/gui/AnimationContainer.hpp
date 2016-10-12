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
    std::map<std::string, float> m_variables;
    Animation* m_updatingAni;
    CloneHandler& m_cloneHandler;
    GraphicalObject graphics;

    std::unique_ptr<MenuElement> doClone() const override;
    void doDraw(const DrawParameter& param) override;

    float onGetValueOf(const std::string& name) const override;
    void onSetValueOf(const std::string& name, const float value) override;

public:
    AnimationContainer(const sf::Vector2f& position, 
                       const sf::Vector2f& offset,
                       int id,
                       CloneHandler& cloneHandler);
    AnimationContainer(AnimationContainer&& toMove);
    void bindAnimation(std::unique_ptr<Animation> animation);

    void update(const sf::RenderWindow& screen, const float time, const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0)) override;
    void updateLayout(const sf::Vector2f& screenSize) override;
};

#endif