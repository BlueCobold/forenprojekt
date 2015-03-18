#ifndef FPSCOUNTER_HPP
#define FPSCOUNTER_HPP

#include <SFML/System/Clock.hpp>

class FPSCounter
{
private:
    
    int m_fps;

    float m_frameCounter;

    sf::Clock m_clock;

public:
    
    FPSCounter();
    
    void update();

    int getFPS();
};

#endif