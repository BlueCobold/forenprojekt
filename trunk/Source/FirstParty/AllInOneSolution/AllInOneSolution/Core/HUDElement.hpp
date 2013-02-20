#ifndef HUDELEMENT_HPP
#define HUDELEMENT_HPP

#include "gui\LineLabel.hpp"

class ResourceManager;

/// Basis class for HUD
class HUDElement
{
protected:

    LineLabel m_label;

    BitmapFont* m_bitmapfont;

    ResourceManager& m_resourceManager;

    sf::Vector2f m_position;

public:
    HUDElement(ResourceManager& resourceManager, const sf::Vector2f& position, const float rotation, std::string bitmapFont, std::string text = "");

    virtual void update(const DrawParameter& params);

    virtual void draw(const DrawParameter& params);

    void setPosition(sf::Vector2f position);

    void setText(std::string text);
};

#endif
