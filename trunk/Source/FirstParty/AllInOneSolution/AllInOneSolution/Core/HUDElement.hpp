#ifndef HUDELEMENT_HPP
#define HUDELEMENT_HPP

#include "gui\LineLabel.hpp"

class ResourceManager;

enum VerticalReference {VR_Top, VR_Bottom, VR_Center};
enum HorizontalReference {HR_Left, HR_Right, HR_Center};

/// Basis class for HUD
class HUDElement : protected LineLabel
{
public:

protected:

    ResourceManager& m_resourceManager;

    sf::Vector2f m_screenPosition;

    HorizontalReference m_horizontalReference;

    VerticalReference m_verticalReference;

public:
    HUDElement(ResourceManager& resourceManager, const sf::Vector2f& position, const float rotation, std::string bitmapFont,
    HorizontalReference hReference = HR_Left, VerticalReference vReference = VR_Top, std::string text = "");

    virtual void update(const DrawParameter& params);

    virtual void draw(const DrawParameter& params);

    void setScreenPosition(sf::Vector2f position, HorizontalReference hReference = HR_Left, VerticalReference vReference = VR_Top);

    virtual void setText(const std::string& text);

    void setScreenPositionPercent(HorizontalReference hReference, float hRefPercent, VerticalReference vReference, float vRefPercent, sf::Vector2f screenSize);
};

#endif
