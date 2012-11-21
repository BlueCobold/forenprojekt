#include "Ball.hpp"

Ball::Ball(b2CircleShape ballShape, float xPos, float yPos, float radius, 
         float density, float friction, sf::Texture texture)
{
    m_bodyDef.type = b2_dynamicBody;
    m_bodyDef.position.Set(xPos, yPos);

    m_fixtureDef.density = density;
    m_fixtureDef.friction = friction;

    m_ballShape = ballShape;
    m_ballShape.m_radius = radius;
    m_ballShape.m_p.Set(0.0f,0.0f);

    bindShape(ballShape);
    bindTexture(texture);
}

Ball::~Ball(void)
{
}

void Ball::setNewPosition(float x, float y)
{
    m_bodyDef.position.Set(x, y);
}

void Ball::setNewPosition(b2Vec2 position)
{
    m_bodyDef.position.Set(position.x, position.y);
}