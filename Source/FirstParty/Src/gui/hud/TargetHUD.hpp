#ifndef TARGETHUD_HPP
#define TARGETHUD_HPP

#include "LabelHUD.hpp"

/// 
class TargetHUD : public LabelHUD
{
    int m_totalTarget;
    int m_remainingTarget;

    sf::Sprite m_targetTexture;
    ResourceManager& m_resourceManager;

    void doDraw(const DrawParameter& params) override;
    void updated(const DrawParameter& params) override;

public:
    TargetHUD(ResourceManager& resourceManager,
              const sf::Vector2f& position,
              const sf::Vector2f& offset,
              const BitmapFont* bitmapFont);

    void setTargets(int totalTarget,int remainingTarget);
};

#endif
