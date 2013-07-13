#pragma once

#ifndef GRAVITYGOODIE_HPP
#define GRAVITYGOODIE_HPP

#include "Goodie.hpp"
#include <Box2D/Common/b2Math.h>

class GravityGoodie : public Goodie
{
private:
    float m_gravityFactor;
    b2Vec2 m_normalGravity;
    b2Vec2 m_currentGravity;

public:
    GravityGoodie(const sf::Keyboard::Key key, const float gravityFactor);
    
    void update(const float elapsedTime);

    void setNormalGravity(const b2Vec2& normalGravity);

    b2Vec2 getGravity();
};

#endif // GRAVITYGOODIE_HPP