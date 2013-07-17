#pragma once

#ifndef GRAVITYGOODIE_HPP
#define GRAVITYGOODIE_HPP

#include "Goodie.hpp"
#include <Box2D/Common/b2Math.h>

class GravityGoodie : public Goodie
{
private:
    float m_gravityFactor;
    b2Vec2& m_currentGravity;

public:
    GravityGoodie(const sf::Keyboard::Key key, const float gravityFactor, b2Vec2& m_currentGravity);
    
    virtual void update(const float elapsedTime) override;

    b2Vec2 getGravity() const;
};

#endif // GRAVITYGOODIE_HPP
