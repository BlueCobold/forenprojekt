#pragma once

#include <Box2D/Box2D.h>

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