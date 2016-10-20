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
    InvulnerableGoody(const sf::Keyboard::Key key, const float durationTime, const Ball* const& ball, const int charge) : 
#ifdef TOUCHSIM
        Goody(key, Goody::InvulnerableGoody, sf::FloatRect(0.f, 114.f, 56.f, 64.f), durationTime, 0, charge),
#else
        Goody(key, Goody::InvulnerableGoody, durationTime, 0, charge),
#endif
        m_ball(ball)
    { }

    bool calculateCanActivate() const override
    {
        return !m_ball->blownUp() && !m_ball->getBallLost();
    }
};

#endif // INVULNERABLE_GOODY_HPP
