#include "Animation.hpp"

#include <SFML/Graphics/Rect.hpp>

Animation::Animation()
{
}

Animation::Animation(const bool infinite, const float min, const float max, const float step, const unsigned int numFrames
    , const unsigned int frameWidth, const unsigned int frameHeight) :
    m_infinite(infinite),
    m_min(min),
    m_max(max),
    m_step(step),
    m_numFrames(numFrames),
    m_sleep(true),
    m_frame(0),
    m_frameWidth(frameWidth),
    m_frameHeight(frameHeight)
{
}

Animation::~Animation()
{
}

void Animation::update(const float value)
{
    if(!m_sleep)
    {
        if(m_infinite)
            calculateFrameInfinte(value);
        else
            calculateFrame(value);

        m_textureRect = sf::IntRect(sf::IntRect((m_frame * m_frameWidth), 0, m_frameWidth, m_frameHeight));
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
    float max = m_step * m_numFrames;
    while(value > max)
        value -= max;
    while(value < m_min)
        value += max;

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

const sf::IntRect Animation::getTextureRect()
{
    return sf::IntRect(sf::IntRect((m_frame * m_frameWidth), 0, m_frameWidth, m_frameHeight));
}
