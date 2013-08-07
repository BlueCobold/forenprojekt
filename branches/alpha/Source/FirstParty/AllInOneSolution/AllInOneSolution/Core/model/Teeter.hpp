#pragma once

#ifndef TEETER_HPP
#define TEETER_HPP

#include "Entity.hpp"

/// This class will be used to bind the physical setting and the control to a entity of a teeter
class Teeter : public Entity
{
private:
    float m_lastMouseX;
    float m_lastTime;
    float m_mouseScale;

public:

    Teeter(const float mouseScale);
    ~Teeter();

    virtual void update(const float value) override;

    void adaptToMouse();
};

#endif //TEETER_HPP

