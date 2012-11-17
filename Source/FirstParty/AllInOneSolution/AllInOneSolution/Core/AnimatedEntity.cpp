#include "AnimatedEntity.hpp"


AnimatedEntity::AnimatedEntity()
{
}


AnimatedEntity::~AnimatedEntity()
{
}

void AnimatedEntity::update(const float value)
{
    Entity::update();

    m_animation.update(value);

    m_sprite.setTextureRect(m_animation.getTextureRect());
}

void AnimatedEntity::bindAnimation(const bool infinite, const float min, const float max, const float step, const unsigned int numFrames
    , const unsigned int frameWidth, const unsigned int frameHeight)
{
    m_animation = Animation(infinite, min, max, step, numFrames, frameWidth, frameHeight);
}