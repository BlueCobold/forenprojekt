#pragma once

#ifndef PHYSICAL_OBJECT_HPP
#define PHYSICAL_OBJECT_HPP

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Collision/Shapes/b2Shape.h>

#include <vector>
#include <memory> // unique_ptr

/// This class will be used to give a object a binding to
/// a physical body
class PhysicalObject
{
public:
    /// Same as a default Constructor
    PhysicalObject() :
      m_shapes()
    { }
    virtual ~PhysicalObject()
    {
        // HACK: since VS10 can't handle unique_ptrs correctly
        // LEAK!!!!!!11!111
    }

    /// Overloaded Function this is for a b2CircleShape
    void bindBody(b2Body* body, std::vector<b2Shape*> shapes)
    {
        m_body = body;
        m_shapes.swap(shapes);
    }
    
protected:

    b2Body* m_body;

    std::vector<b2Shape*> m_shapes; // HACK: since VS10 can't handle unique_ptrs correctly
};

#endif // PHYSICAL_OBJECT_HPP
