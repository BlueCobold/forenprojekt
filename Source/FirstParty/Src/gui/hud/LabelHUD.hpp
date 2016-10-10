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
    void doDraw(const DrawParameter& params) override;

public:
    LabelHUD(const sf::Vector2f& position,
             const sf::Vector2f& offset,
             const BitmapFont* bitmapFont,
             const std::string& text = "",
             const LineLabel::Alignment alignment = LineLabel::Left);

    void update(const DrawParameter& params) override;

    void setText(const std::string& text);

    void setPosition(const sf::Vector2f& position, const sf::Vector2f& offset) override;
};

#endif
