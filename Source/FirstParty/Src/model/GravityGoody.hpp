#pragma once

#ifndef GRAVITYGOODY_HPP
#define GRAVITYGOODY_HPP

#include "Goody.hpp"
#include <Box2D/Common/b2Math.h>

class GravityGoody : public Goody
{
public:
    GravityGoody(const sf::Keyboard::Key key, const float gravityFactor, b2Vec2& m_currentGravity, const int charge);

    b2Vec2 getGravity() const;
    const int getBonusPoints() const;

private:
    float m_gravityFactor;
    float m_currentFactor;
    float m_activeStartTime;
    float m_activeTime;
    b2Vec2& m_currentGravity;

    void updated() override;
};

#endif // GRAVITYGOODY_HPP
