#pragma once

#ifndef TEETER_HPP
#define TEETER_HPP

#include "Entity.hpp"

/// This class will be used to bind the physical setting and the control to a entity of a teeter
class Teeter : public Entity
{
private:
    sf::Vector2f m_lastMousePos;
    float m_lastTime;
    float m_mouseScale;

    bool m_invertAxis;
    bool m_useVerticalAxis;

public:

    Teeter(const float mouseScale);
    ~Teeter();

    virtual void update(const float value) override;

    void adaptToMouse();
    void setControll(const bool invertAxis, const bool useVerticalAxis);
};

#endif //TEETER_HPP

