#include "Ball.hpp"


Ball::Ball(b2World& world)
{
    ballDef.type = b2_dynamicBody;
    ballDef.position.Set(0.0f, 4.0f);
    b2Body* ballBody = world.CreateBody(&ballDef);
}


Ball::~Ball(void)
{
}
