#ifndef FPSCOUNTERHUD_HPP
#define FPSCOUNTERHUD_HPP

#include "gui\LineLabel.hpp"
#include "FPSCounter.hpp"
#include "HUDElement.hpp"

class ResourceManager;


///
class FPSCounterHUD : public HUDElement
{
private:
    
    FPSCounter m_fpsCounter;

public:

    FPSCounterHUD(const sf::Vector2f& position, const float rotation, BitmapFont* bitmapFont);
    
    void update(const DrawParameter& params);

};

#endif
