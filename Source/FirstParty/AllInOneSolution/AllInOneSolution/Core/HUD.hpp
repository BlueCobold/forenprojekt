#ifndef HUD_HPP
#define HUD_HPP

#include "FPSCounterHUD.hpp"
#include "TargetHUD.hpp"
#include <memory> // unique_ptr

class ResourceManager;
class Config;
class Level;

namespace sf
{
    class RenderWindow;
}

/// for Displaying all the HUD Elements
class HUD
{
private:

    sf::RenderTarget& m_screen;

    ResourceManager& m_resourceManager;

    FPSCounterHUD m_fpsCounter;
    bool m_fpsShow;

    TargetHUD m_target;

public:
    HUD(sf::RenderTarget& screen, ResourceManager& resourceManager, Config& config);

    void update(Level* level);

    void draw();

};

#endif