#ifndef HUD_HPP
#define HUD_HPP

#include "FPSCounterHUD.hpp"
#include "TargetHUD.hpp"
#include "ArrowHUD.hpp"
#include "PointsHUD.hpp"
#include "BallHUD.hpp"
#include "TimeHUD.hpp"
#include "GoodyHUD.hpp"
#include "BatteryHUD.hpp"

#include <memory> // unique_ptr

class ResourceManager;
class AppConfig;
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
    TargetHUD m_target;
    PointsHUD m_points;
    ArrowHUD m_arrow;
    BallHUD m_ball;
    TimeHUD m_time;
    BatteryHUD m_battery;

    bool m_fpsShow;
    bool m_ballShow;
    bool m_timeShow;
    bool m_batteryStateShow;

    GoodyHUD m_gravityGoody;
    GoodyHUD m_invulnerableGoody;
    GoodyHUD m_extraBallGoody;
    GoodyHUD m_extraTimeGoody;

    AppConfig& m_config;
public:
    HUD(ResourceManager& resourceManager, AppConfig& config);

    void skipInterpolation();

    void update(const Level* level, const float time);
    void restartAt(const float time);

    void draw(const DrawParameter& params);

    void onEnter(Level* level);

private:
    void setBallShow(bool ballShow);
    void setTimeShow(bool timeShow);
};

#endif
