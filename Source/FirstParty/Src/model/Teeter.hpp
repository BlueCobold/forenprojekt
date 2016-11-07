#pragma once

#ifndef TEETER_HPP
#define TEETER_HPP

#include "Entity.hpp"

/// This class will be used to bind the physical setting and the control to a entity of a teeter
class Teeter : public Entity
{
public:
    Teeter(const float mouseScale, CloneHandler& cloneHandler);
    ~Teeter();

    void adaptToMouse();
    void setControl(const bool invertAxis, const bool useVerticalAxis);

private:
    sf::Vector2f m_lastMousePos;
    double m_lastTime;
    float m_mouseScale;

    bool m_invertAxis;
    bool m_useVerticalAxis;

    void timeUpdated() override;
};

#endif //TEETER_HPP

