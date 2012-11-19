#include "Ball.hpp"


Ball::Ball(b2CircleShape ballShape)
{
    m_ballShape = ballShape;
    bindShape(ballShape);
}


Ball::~Ball(void)
{
}

void Ball::update()
{
    m_sprite.setPosition(m_bodyDef.position.x,m_bodyDef.position.y);
}

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite,states);
}

void Ball::setNewPosition(float x, float y)
{
    m_bodyDef.position.Set(x, y);
}

void Ball::setNewPosition(b2Vec2 position)
{
    m_bodyDef.position.Set(position.x, position.y);
}