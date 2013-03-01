#pragma once

#ifndef TRANSITION_HPP
#define TRANSITION_HPP

#include "../Drawable.hpp"
#include "../../animation/TimedObject.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>

class Transition : public Drawable, public TimedObject
{
public:

    Transition(
        const sf::Texture* sourceTexture,
        const sf::Texture* targetTexture,
        const float duration) :
        m_sourceTexture(sourceTexture),
        m_targetTexture(targetTexture),
        m_duration(duration),
        m_isFinished(false)
    {
        TimedObject::restartAt(m_frameTime.getElapsedTime().asSeconds());
    }

    virtual ~Transition()
    { }

    virtual void update()
    {
        TimedObject::updateCurrentTime(m_frameTime.getElapsedTime().asSeconds());
        if (getPassedTime() > m_duration)
            m_isFinished = true;
    }

    bool isFinished() const { return m_isFinished; };

protected:

    const float getProgress() const
    {
        float elapsed = getPassedTime();
        if (elapsed > m_duration)
            return 1.f;
        return elapsed / m_duration;
    }

    const sf::Texture* getTargetTexture() const { return m_targetTexture; };
    const sf::Texture* getSourceTexture() const { return m_sourceTexture; };
    const float getDuration() const { return m_duration; }

private:

    Transition( const Transition& other );
    Transition& operator=( const Transition& );

    const sf::Texture* m_targetTexture;
    const sf::Texture* m_sourceTexture;
    sf::Clock m_frameTime;
    float m_duration;
    bool m_isFinished;
};

#endif // TRANSITION_HPP
