#pragma once

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Animation.hpp"
#include "GraphicalObject.hpp"
#include "PhysicalObject.hpp"
#include "animation/TimedObject.hpp"

#include <memory> // std::unique_ptr
#include <string>

/// This class will be used to draw objects that have a binding
/// to Box2D 
class Entity : public PhysicalObject, public GraphicalObject, public TimedObject
{
private:

    std::unique_ptr<Animation> m_animation;
    std::string m_name;
    bool m_autoRotate;

public:

    Entity();
    virtual ~Entity();

    virtual void update(const float value);

    void setName(std::string name)
    {
        m_name = name;
    }

    void bindAnimation(std::unique_ptr<Animation> m_animation, bool autoRotate = false);

    const std::unique_ptr<Animation> getAnimation();

protected:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // ENTITY_HPP
