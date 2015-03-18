#pragma once

#ifndef PARTICLE_TRAIL_HPP
#define PARTICLE_TRAIL_HPP

#include "Animation.hpp"
#include "../rendering/Drawable.hpp"

#include <memory>
#include <vector>

class ParticleTrail : public Drawable
{
public:
    ParticleTrail(std::unique_ptr<Animation> animationTemplate, float density, float speedMin);

    virtual void draw(const DrawParameter& params) override;

    void moveTo(const float x, const float y);
    void setTo(const float x, const float y);
    void update();
    Animation* getCurrentAnimation();

    float getSpeedMin() const { return m_speedMin; }

private:
    Animation* m_current;
    float m_density;
    float m_speedMin;
    float m_lastX;
    float m_lastY;
    std::unique_ptr<Animation> m_template;
    std::vector<std::unique_ptr<Animation>> m_runningAnimations;
};

#endif
