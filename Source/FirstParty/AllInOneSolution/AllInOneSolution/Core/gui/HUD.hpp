#ifndef HUD_HPP
#define HUD_HPP

#include "FPSCounterHUD.hpp"
#include "TargetHUD.hpp"
#include "ArrowHUD.hpp"
#include "PointsHUD.hpp"
#include "BallHUD.hpp"

#include <memory> // unique_ptr

class ResourceManager;
class Config;
class Level;

namespace sf
{
    class RenderWindow;
}

/// for Displaying all the HUD Elements
class HUD : public Drawable
{
private:

    FPSCounterHUD m_fpsCounter;
    bool m_fpsShow;

    TargetHUD m_target;

    PointsHUD m_points;

    ArrowHUD m_arrow;

    BallHUD m_ball;
    bool m_ballShow;

public:
    HUD(ResourceManager& resourceManager, Config& config);

    void update(const Level* level, const float time);
    void restartAt(const float time);

    void draw(const DrawParameter& params);

private:
    void setBallShow(bool ballShow);
};

#endif
