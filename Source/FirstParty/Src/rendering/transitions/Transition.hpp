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
        const double duration,
        const sf::Vector2u& size) :
        m_size(size),
        m_targetTexture(targetTexture),
        m_sourceTexture(sourceTexture),
        m_duration(duration),
        m_isFinished(false)
    {
        restartAt(m_frameTime.getElapsedTime().asSeconds());
    }

    virtual ~Transition()
    { }

    void update()
    {
        TimedObject::updateCurrentTime(m_frameTime.getElapsedTime().asSeconds());
        if(getPassedTime() > m_duration)
            m_isFinished = true;
        updated();
    }

    bool isFinished() const { return m_isFinished; };

protected:
    virtual void updated()
    { }

    const float getProgress() const
    {
        auto elapsed = getPassedTime();
        if(elapsed > m_duration)
            return 1.f;
        return static_cast<float>(elapsed / m_duration);
    }

    const sf::Texture* getTargetTexture() const { return m_targetTexture; };
    const sf::Texture* getSourceTexture() const { return m_sourceTexture; };
    const double getDuration() const { return m_duration; }
    const sf::Vector2u& getSize() const { return m_size; }

private:
    const sf::Vector2u m_size;
    const sf::Texture* m_targetTexture;
    const sf::Texture* m_sourceTexture;
    sf::Clock m_frameTime;
    double m_duration;
    bool m_isFinished;

    Transition( const Transition& other );
    Transition& operator=( const Transition& );
};

#endif // TRANSITION_HPP
