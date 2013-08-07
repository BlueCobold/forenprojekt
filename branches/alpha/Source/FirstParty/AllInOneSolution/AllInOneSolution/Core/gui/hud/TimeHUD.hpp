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

public:
    TimeHUD(ResourceManager& resourceManager,
        const sf::Vector2f& position,
        const float rotation,
        BitmapFont* bitmapFont,
        float hReference = HUDElement::Left,
        float vReference = HUDElement::Top);

    void update(const DrawParameter& params);

    void draw(const DrawParameter& params);

    void setTime(int totalSeconds);
};

#endif //TIMEHUD_HPP