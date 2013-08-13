#pragma once

#ifndef INVULNERABLE_GOODY_HPP
#define INVULNERABLE_GOODY_HPP

#include "Goody.hpp"
#include "Ball.hpp"
#include <Box2D/Common/b2Math.h>

class InvulnerableGoody : public Goody
{
private:
    const Ball* const& m_ball;

public:
    InvulnerableGoody(const sf::Keyboard::Key key, const int price, const float durationTime, const Ball* const& ball, const int charge)
        : Goody(key, Goody::InvulnerableGoody, price, durationTime, 0, charge),
        m_ball(ball)
    { }

    virtual bool canActivate() const override
    {
        return !m_ball->blownUp() && !m_ball->getBallLost();
    }
};

#endif // INVULNERABLE_GOODY_HPP
