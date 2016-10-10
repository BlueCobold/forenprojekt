#ifndef TIMEHUD_HPP
#define TIMEHUD_HPP

#include "LabelHUD.hpp"

/// 
class TimeHUD : public LabelHUD
{
private:
    std::string m_seconds;
    std::string m_minutes;

    int m_totalSeconds;

    sf::Sprite m_clock;
    ResourceManager& m_resourceManager;

    void doDraw(const DrawParameter& params) override;

public:
    TimeHUD(ResourceManager& resourceManager,
            const sf::Vector2f& position,
            const sf::Vector2f& offset,
            const BitmapFont* bitmapFont);

    void update(const DrawParameter& params);

    void setTime(int totalSeconds);
};

#endif //TIMEHUD_HPP