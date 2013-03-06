#ifndef ARROW_HPP
#define ARROW_HPP

#include "HUDElement.hpp"

class ArrowHUD : public HUDElement
{
private:

    sf::Sprite m_arrowTexture;

    ResourceManager& m_resourceManager;

    bool m_outOfView;

public:

    void update(const DrawParameter& params);

    void draw(const DrawParameter& params);

    ArrowHUD(ResourceManager& resourceManager);

    void setBallCoords(sf::Vector2f coords);
};

#endif /// ARROW_HPP