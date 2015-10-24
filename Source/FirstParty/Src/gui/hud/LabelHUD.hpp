#ifndef LABELHUD_HPP
#define LABELHUD_HPP

#include "../LineLabel.hpp"
#include "HUDElement.hpp"

class ResourceManager;

/// this class draw a label on HUD screen
class LabelHUD : public HUDElement
{
protected:

    LineLabel m_label;

public:
    LabelHUD(const sf::Vector2f& position,
             const sf::Vector2f& offset,
             const BitmapFont* bitmapFont,
             const std::string& text = "",
             const LineLabel::Alignment alignment = LineLabel::Left);

    virtual ~LabelHUD()
    { }

    virtual void update(const DrawParameter& params);

    virtual void draw(const DrawParameter& params);

    void setText(const std::string& text);

    virtual void setPosition(const sf::Vector2f& position, const sf::Vector2f& offset) override;
};

#endif
