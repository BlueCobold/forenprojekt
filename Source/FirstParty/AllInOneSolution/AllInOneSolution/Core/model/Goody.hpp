#pragma once

#ifndef GOODY_HPP
#define GOODY_HPP

#include "../Input.hpp"

class Goody
{
private:
    sf::Keyboard::Key m_key;
    bool m_active;
    int m_charges;
    float m_cooldownTime;
    float m_nextUseTime;
    float m_durationTime;
    float m_durationUntilTime;

public:
    Goody(const sf::Keyboard::Key key, const float durationTime = 0, const float cooldownTime = 0, 
           const int charges = -1);

    virtual void update(const float elapsedTime);

    virtual bool isActive() const;
    virtual bool canActivate() const;
};

#endif
