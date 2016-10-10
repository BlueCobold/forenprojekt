#ifndef INTERACTIVELABEL_HPP
#define INTERACTIVELABEL_HPP

#include "LineLabel.hpp"
#include "ToolTip.hpp"

#pragma once

// this class add a ToolTip to a standard LineLabel
class InteractiveLabel : public LineLabel
{
private:
    ToolTip m_toolTip;
    bool m_showToolTip;

    std::unique_ptr<MenuElement> doClone() const override;
    void doDraw(const DrawParameter& params) override;

public:
    //InteractiveLabel();

    InteractiveLabel(const std::string& text,
        const sf::Vector2f& position,
        const sf::Vector2f& offset,
        const float rotation,
        const BitmapFont* font,
        const std::string& language,
        const Alignment alignment = Left,
        int id = -1);

    void setToolTip(const ToolTip& toolTip);
    void setToolTipText(const std::string& text, const std::string& replacement = "");

    void update(const sf::RenderWindow& screen, const float time, const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0)) override;
    void drawAdditionalForeground(const DrawParameter& params) override;

    void setLanguage(const std::string& language) override;
};

#endif