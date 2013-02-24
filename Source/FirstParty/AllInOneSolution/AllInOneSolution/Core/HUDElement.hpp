#ifndef HUDELEMENT_HPP
#define HUDELEMENT_HPP

#include "gui\LineLabel.hpp"

class ResourceManager;

/// Basis class for HUD
class HUDElement
{
public:
    
    enum VerticalReference {VR_Top, VR_Bottom, VR_Center};
    enum HorizontalReference {HR_Left, HR_Right, HR_Center};

protected:

    LineLabel m_label;

    BitmapFont* m_bitmapfont;

    ResourceManager& m_resourceManager;

    sf::Vector2f m_position;

    HorizontalReference m_horizontalReference;

    VerticalReference m_verticalReference;

public:
    HUDElement(ResourceManager& resourceManager, const sf::Vector2f& position, const float rotation, std::string bitmapFont,
    HorizontalReference hReference = HR_Left, VerticalReference vReference = VR_Top, std::string text = "");

    virtual void update(const DrawParameter& params);

    virtual void draw(const DrawParameter& params);

    void setPosition(sf::Vector2f position, HorizontalReference hReference = HR_Left, VerticalReference vReference = VR_Top);

    void setText(std::string text);
};

#endif