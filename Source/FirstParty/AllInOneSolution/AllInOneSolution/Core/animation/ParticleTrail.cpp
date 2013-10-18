#include "ParticleTrail.hpp"

#include <cmath>

ParticleTrail::ParticleTrail(std::unique_ptr<Animation> animationTemplate, float density, float speedMin) :
    m_template(std::move(animationTemplate)),
    m_density(utility::toMeter(density)),
    m_speedMin(speedMin),
    m_current(nullptr)
{
}
    
void ParticleTrail::draw(const DrawParameter& params)
{
    for(auto ani = begin(m_runningAnimations); ani != end(m_runningAnimations); ++ani)
        (*ani)->draw(params);
}

void ParticleTrail::moveTo(const float x, const float y)
{
    float dx = x - m_lastX;
    float dy = y - m_lastY;
    float distance = sqrtf(dx * dx + dy * dy);
    if(distance < FLT_EPSILON)
        return;

    auto count = distance / m_density;
    if(count < 1)
        return;

    dx /= count;
    dy /= count;
    for(int i = 0; i < static_cast<int>(count); i++)
    {
        auto ani = std::unique_ptr<Animation>(m_template->clone());
        ani->setPosition(utility::toPixel<float>(m_lastX + i * dx), utility::toPixel<float>(m_lastY + i * dy));
        m_runningAnimations.push_back(std::move(ani));
    }
    m_lastX += dx * static_cast<int>(count);
    m_lastY += dy * static_cast<int>(count);
}

void ParticleTrail::setTo(const float x, const float y)
{
    m_lastX = x;
    m_lastY = y;
}

void ParticleTrail::update()
{
    auto it = begin(m_runningAnimations);
    while(it != end(m_runningAnimations))
    {
        auto ani = it->get();
        m_current = ani;
        ani->update();
        m_current = nullptr;
        if(ani->isStopped())
            it = m_runningAnimations.erase(it);
        else
            ++it;
    }
}

Animation* ParticleTrail::getCurrentAnimation()
{
    return m_current;
}
