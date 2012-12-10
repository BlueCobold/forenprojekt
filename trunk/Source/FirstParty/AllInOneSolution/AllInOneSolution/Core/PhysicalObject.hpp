#pragma once

#ifndef PHYSICAL_OBJECT_HPP
#define PHYSICAL_OBJECT_HPP

#include "animation/OrientedObject.hpp"
#include "animation/provider/ValueProvider.hpp"

#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Collision/Shapes/b2Shape.h>
#include <Box2D/Dynamics/b2Body.h>

#include <memory> // unique_ptr
#include <vector>

/// This class will be used to give a object a binding to
/// a physical body
class PhysicalObject : public OrientedObject
{
private:

    std::unique_ptr<ValueProvider> m_rotation;
    std::unique_ptr<ValueProvider> m_xPositionProvider;
    std::unique_ptr<ValueProvider> m_yPositionProvider;
    b2Vec2 m_basePosition;

public:

    PhysicalObject()
    { }
    virtual ~PhysicalObject()
    { }

    /// Bind the body
    void bindBody(b2Body* body)
    {
        m_body = body;
        m_basePosition = m_body->GetPosition();
    }

    void unbindBody()
    {
        m_body->GetWorld()->DestroyBody(m_body);
    }

    void bindRotationController(std::unique_ptr<ValueProvider> provider)
    {
        m_rotation = std::move(provider);
    }

    void bindPositionController(std::unique_ptr<ValueProvider> x, std::unique_ptr<ValueProvider> y)
    {
        m_xPositionProvider = std::move(x);
        m_yPositionProvider = std::move(y);
    }

    virtual float getAngle() const
    {
        return m_body->GetAngle();
    }

    void updateKinematics(const float value, const float delta);

protected:

    b2Body* m_body;
};

#endif // PHYSICAL_OBJECT_HPP
