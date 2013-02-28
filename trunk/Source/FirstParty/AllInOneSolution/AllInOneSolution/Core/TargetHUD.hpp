#ifndef TARGETHUD_HPP
#define TARGETHUD_HPP

#include "HUDElement.hpp"

/// 
class TargetHUD : public HUDElement
{
private:
    int m_totalTarget;

    int m_remainingTarget;

    sf::Sprite m_targetTexture;

    float xDiff;

public:
    TargetHUD(ResourceManager& resourceManager, const sf::Vector2f& position, const float rotation, std::string bitmapFont,
		float hReference = HUDElement::Left, float vReference = HUDElement::Top);

    void update(const DrawParameter& params);

    void draw(const DrawParameter& params);

    void setTargets(int totalTarget,int remainingTarget);
};

#endif