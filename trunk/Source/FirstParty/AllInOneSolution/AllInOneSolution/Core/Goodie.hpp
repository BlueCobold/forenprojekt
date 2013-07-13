#pragma once

#ifndef GOODIE_HPP
#define GOODIE_HPP

#include "Input.hpp"

class Goodie
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
    Goodie(const sf::Keyboard::Key key, const float durationTime = 0, const float cooldownTime = 0, 
           const int chrages = -1);

    virtual void update(const float elapsedTime);

    const bool isActive() const;
};

#endif