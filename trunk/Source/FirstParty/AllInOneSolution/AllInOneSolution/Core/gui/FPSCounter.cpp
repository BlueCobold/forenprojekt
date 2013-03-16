#include "FPSCounter.hpp"

FPSCounter::FPSCounter() : 
m_fps(0), 
m_frameCounter(0.0f)
{
    m_clock.restart();
}

void FPSCounter::update()
{
    m_frameCounter++;
    if(m_clock.getElapsedTime().asSeconds() >= 1.0f)
    {
        m_fps = static_cast<int>((m_frameCounter / m_clock.getElapsedTime().asSeconds()));
        m_clock.restart();
        m_frameCounter = 0;
    }
}

int FPSCounter::getFPS()
{
    return m_fps;
}