#ifndef TARGETHUD_HPP
#define TARGETHUD_HPP

#include "gui\LineLabel.hpp"

class ResourceManager;

class TargetHUD
{
private:
    int m_totalTarget;

    int m_remainingTarget;

    LineLabel m_label;

    BitmapFont* m_bitmapfont;

    ResourceManager& m_resourceManager;

    sf::Vector2f m_position;

public:

    void update(const DrawParameter& params);

    void draw(const DrawParameter& params);
};

#endif