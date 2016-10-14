#ifndef FPSCOUNTERHUD_HPP
#define FPSCOUNTERHUD_HPP

#include "../LineLabel.hpp"
#include "../FPSCounter.hpp"
#include "LabelHUD.hpp"

class ResourceManager;

///
class FPSCounterHUD : public LabelHUD
{
    FPSCounter m_fpsCounter;
    
    void updated(const DrawParameter& params) override;

public:
    FPSCounterHUD(const sf::Vector2f& position,
                  const sf::Vector2f& offset,
                  const BitmapFont* bitmapFont);
};

#endif
