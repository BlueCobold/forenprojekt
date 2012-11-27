#pragma once

#ifndef PHYSICAL_OBJECT_HPP
#define PHYSICAL_OBJECT_HPP

#include "animation/OrientedObject.hpp"

#include <Box2D/Collision/Shapes/b2Shape.h>
#include <Box2D/Dynamics/b2Body.h>

#include <memory> // unique_ptr
#include <vector>

/// This class will be used to give a object a binding to
/// a physical body
class PhysicalObject : public OrientedObject
{
public:

    PhysicalObject()
    { }
    virtual ~PhysicalObject()
    { }

    /// Bind the body
    void bindBody(b2Body* body)
    {
        m_body = body;
    }
    
    virtual float getAngle() const
    {
        return m_body->GetAngle();
    }

protected:

    b2Body* m_body;

};

#endif // PHYSICAL_OBJECT_HPP
