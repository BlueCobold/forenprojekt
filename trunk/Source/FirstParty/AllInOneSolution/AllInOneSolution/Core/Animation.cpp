#include "Animation.hpp"

#include <SFML/Graphics/Rect.hpp>

Animation::Animation() :
    m_infinite(false),
    m_min(0.f),
    m_max(0.f),
    m_step(0.f),
    m_numFrames(0),
    m_sleep(true),
    m_frame(0),
    m_frameWidth(0),
    m_frameHeight(0)
{ }

Animation::Animation(const bool infinite, const float min, const float step,
    const unsigned int numFrames, const unsigned int frameWidth, const unsigned int frameHeight) :
    m_infinite(infinite),
    m_min(min),
    m_max(step*numFrames),
    m_step(step),
    m_numFrames(numFrames),
    m_sleep(true),
    m_frame(0),
    m_frameWidth(frameWidth),
    m_frameHeight(frameHeight)
{ }

Animation::~Animation()
{ }

void Animation::update(const float value)
{
    if(!m_sleep)
    {
        if(m_infinite)
            calculateFrameInfinte(value);
        else
            calculateFrame(value);

        m_textureRect = sf::IntRect(m_frame * m_frameWidth, 0, m_frameWidth, m_frameHeight);
    }
}

void Animation::calculateFrame(const float value)
{
    if(value > m_max)
        m_sleep = true;
    else if(value < m_min)
        m_sleep = true;
    else
        m_frame = static_cast<unsigned int>(value / m_step);
}

void Animation::calculateFrameInfinte(float value)
{
    while(value > m_max)
        value -= m_max;
    while(value < m_min)
        value += m_max;

    m_frame = static_cast<unsigned int>(value / m_step);
}

void Animation::start()
{
    m_sleep = false;
}

void Animation::pause()
{
    m_sleep = true;
}

const sf::IntRect& Animation::getTextureRect()
{
    return m_textureRect;
}
