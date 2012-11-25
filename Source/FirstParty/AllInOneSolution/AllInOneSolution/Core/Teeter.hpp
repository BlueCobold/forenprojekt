#pragma once

#ifndef TEETER_HPP
#define TEETER_HPP

#include "Entity.hpp"

/// This class will be used to bind the physical setting and the control to a entity of a teeter
class Teeter : public Entity
{
public:

    Teeter(const float32 x, const float32 y, const float centerX, const float centerY, 
        const b2FixtureDef& fixtureDef, b2World& world);
    ~Teeter();

    virtual void update(const float value);

};

#endif //TEETER_HPP

