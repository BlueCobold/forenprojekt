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

    FPSCounterHUD(ResourceManager& resourceManager, const sf::Vector2f& position, const float rotation, std::string bitmapFont);
    
    void update(const DrawParameter& params);

};

#endif