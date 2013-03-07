#ifndef ARROW_HPP
#define ARROW_HPP

#include "HUDElement.hpp"

class ArrowHUD : public HUDElement
{
private:
    static const int ARROW_WIDTH = 64;

    sf::Sprite m_arrow;
    ResourceManager& m_resourceManager;
    sf::Vector2f m_ballPos;

    bool m_outOfView;

public:

    void update(const DrawParameter& params);

    void draw(const DrawParameter& params);

    ArrowHUD(ResourceManager& resourceManager);

    void setBallCoords(const sf::Vector2f& ballPos);
};

#endif /// ARROW_HPP
