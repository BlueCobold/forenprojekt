#pragma once

#ifndef INTERACTIVELABEL_HPP
#define INTERACTIVELABEL_HPP

#include "LineLabel.hpp"
#include "ToolTip.hpp"

// this class add a ToolTip to a standard LineLabel
class InteractiveLabel : public LineLabel
{
private:
    ToolTip m_toolTip;
    bool m_showToolTip;

    std::unique_ptr<MenuElement> onClone() const override;
    void onDrawAdditionalForeground(const DrawParameter& params) override;
    void updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0)) override;

public:
    InteractiveLabel(const std::string& text,
        const ScreenLocation& position,
        const float rotation,
        const BitmapFont* font,
        const Alignment alignment = Left,
        int id = -1);

    void setToolTip(const ToolTip& toolTip);
    void setToolTipText(const std::string& text, const std::string& replacement = "");
};

#endif