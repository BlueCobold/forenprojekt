#ifndef HUD_HPP
#define HUD_HPP

#include "FPSCounterHUD.hpp"
#include "TargetHUD.hpp"
#include "ArrowHUD.hpp"
#include "PointsHUD.hpp"
#include "BallHUD.hpp"
#include "TimeHUD.hpp"
#include "GoodyHUD.hpp"

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

    TimeHUD m_time;
    bool m_timeShow;

    GoodyHUD m_gravityGoody;
    GoodyHUD m_invulnerableGoody;
    GoodyHUD m_extraBallGoody;
    GoodyHUD m_extraTimeGoody;

public:
    HUD(ResourceManager& resourceManager, Config& config);

    void skipInterpolation();

    void update(const Level* level, const float time);
    void restartAt(const float time);

    void draw(const DrawParameter& params);

private:
    void setBallShow(bool ballShow);
    void setTimeShow(bool timeShow);
};

#endif
