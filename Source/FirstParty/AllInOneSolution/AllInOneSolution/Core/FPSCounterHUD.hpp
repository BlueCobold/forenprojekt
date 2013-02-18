#ifndef FPSCOUNTERHUD_HPP
#define FPSCOUNTERHUD_HPP

#include "gui\LineLabel.hpp"
#include "FPSCounter.hpp"

class ResourceManager;

class FPSCounterHUD
{
private:
    
    FPSCounter m_fpsCounter;
    
    LineLabel m_label;

    BitmapFont* m_bitmapfont;

    ResourceManager& m_resourceManager;

    sf::Vector2f m_position;

public:

    FPSCounterHUD(ResourceManager& resourceManager, const std::string& text, const sf::Vector2f& position, const float rotation, std::string bitmapFont);
    
    void update(const DrawParameter& params);

    void draw(const DrawParameter& params);

    void setPosition(sf::Vector2f position);
};

#endif