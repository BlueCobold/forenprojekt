#pragma once

#include <Box2D/Box2D.h>

/// This class will be used to give a object a binding to
/// a physical body
class PhysicalObject
{
public:

    void bindBody(b2Body& body)
    {
        m_body = body;
    }

protected:

    b2Body& m_body;
};