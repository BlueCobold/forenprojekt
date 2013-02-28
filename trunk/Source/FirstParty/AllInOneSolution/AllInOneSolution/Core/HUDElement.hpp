#ifndef HUDELEMENT_HPP
#define HUDELEMENT_HPP

#include "gui\LineLabel.hpp"

class ResourceManager;

/// Basis class for HUD
class HUDElement
{
public:

	static const float Left;
	static const float Right;
	static const float Center;
	static const float Top;
	static const float Middle;
	static const float Bottom;

protected:

    LineLabel m_label;

    BitmapFont* m_bitmapfont;

    ResourceManager& m_resourceManager;

    sf::Vector2f m_position;

    float m_horizontalPercentage;

    float m_verticalPercentage;

public:
    HUDElement(ResourceManager& resourceManager, const sf::Vector2f& position, const float rotation, std::string bitmapFont,
		float horizontalPercentage = HUDElement::Left, float verticalPercentage = HUDElement::Top, std::string text = "");

    virtual void update(const DrawParameter& params);

    virtual void draw(const DrawParameter& params);

    void setPosition(sf::Vector2f position, float horizontalPercentage = HUDElement::Left, float verticalPercentage = HUDElement::Top);

    void setText(std::string text);
};

#endif