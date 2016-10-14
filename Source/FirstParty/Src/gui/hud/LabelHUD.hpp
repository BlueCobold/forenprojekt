#ifndef LABELHUD_HPP
#define LABELHUD_HPP

#include "../LineLabel.hpp"
#include "HUDElement.hpp"

class ResourceManager;

/// this class draw a label on HUD screen
class LabelHUD : public HUDElement
{
    LineLabel m_label;

protected:
    void doDraw(const DrawParameter& params) override;
    void updated(const DrawParameter& params) override;
    void onPositionSet(const sf::Vector2f& position, const sf::Vector2f& offset) override;

public:
    LabelHUD(const sf::Vector2f& position,
             const sf::Vector2f& offset,
             const BitmapFont* bitmapFont,
             const std::string& text = "",
             const LineLabel::Alignment alignment = LineLabel::Left);

    void setText(const std::string& text);
    void setAlignment(const LineLabel::Alignment alignment);
    float getLabelWidth() const;
};

#endif
