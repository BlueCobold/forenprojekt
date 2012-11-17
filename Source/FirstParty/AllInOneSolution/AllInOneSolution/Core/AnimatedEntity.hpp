#pragma once

#ifndef ANIMATED_ENTITY_HPP
#define ANIMATED_ENTITY_HPP

#include "Entity.hpp"
#include "Animation.hpp"

/// This class will be used to combined a Aniamtion and a Entity
class AnimatedEntity : public Entity
{
public:

    AnimatedEntity();
    virtual ~AnimatedEntity();

    void bindAnimation(const bool infinite, const float min, const float max, const float step, const unsigned int numFrames
        , const unsigned int frameWidth, const unsigned int frameHeight);
    
    void update( const float value );

    Animation& getAnimation();

private:

    Animation m_animation;

};

#endif // ANIMATED_ENTITY_HPP
