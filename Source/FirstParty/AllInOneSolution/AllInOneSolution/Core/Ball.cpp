#include "Ball.hpp"


Ball::Ball(b2World& world, const sf::Texture& texture, float radius, float density, float friction)
{
    m_ballDef.type = b2_dynamicBody;
    m_ballDef.position.Set(0.0f, 4.0f);
    b2Body* n_ballBody = world.CreateBody(&m_ballDef);

    m_ballShape.m_radius = radius;
    m_ballShape.m_p.Set(0,0);

    m_ballFixtureDef.shape = &m_ballShape;
    m_ballFixtureDef.density = density;
    m_ballFixtureDef.friction = friction;

    m_ballBody->CreateFixture(&m_ballFixtureDef);

    m_sprite.setTexture(texture);
    m_sprite.setPosition(m_ballDef.position.x,m_ballDef.position.y);
}


Ball::~Ball(void)
{
}

void Ball::update()
{
    m_sprite.setPosition(m_ballDef.position.x,m_ballDef.position.y);
}

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    target.draw(m_sprite,states);
}

void Ball::setNewPosition(float x, float y)
{
    m_ballDef.position.Set(x, y);
}

void Ball::setNewPosition(b2Vec2 position)
{
    m_ballDef.position.Set(position.x, position.y);
}