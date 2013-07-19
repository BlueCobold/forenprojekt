#pragma once

#ifndef GRAVITYGOODY_HPP
#define GRAVITYGOODY_HPP

#include "Goody.hpp"
#include <Box2D/Common/b2Math.h>

class GravityGoody : public Goody
{
private:
    float m_gravityFactor;
    float m_currentFactor;
    b2Vec2& m_currentGravity;

public:
    GravityGoody(const sf::Keyboard::Key key, const float gravityFactor, b2Vec2& m_currentGravity);
    
    virtual void update(const float elapsedTime) override;

    b2Vec2 getGravity() const;
};

#endif // GRAVITYGOODY_HPP
