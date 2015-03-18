#pragma once

#ifndef ORIENTED_OBJECT_HPP
#define ORIENTED_OBJECT_HPP

/// This interface specifies the orientation of an object.
class OrientedObject
{
public:

    virtual float getAngle() const
    {
        return 0;
    }
};

#endif //ORIENTED_OBJECT_HPP
