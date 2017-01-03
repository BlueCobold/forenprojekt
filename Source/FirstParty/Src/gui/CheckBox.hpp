#pragma once

#ifndef CHECKBOX_HPP
#define CHECKBOX_HPP

#include "CheckboxStyle.hpp"
#include "MenuElement.hpp"
#include "ToolTip.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

class CheckBox : public MenuElement
{
public:

    CheckBox(int id, CheckBoxStyle style, const ScreenLocation& position);
    
    bool getChecked();
    void setChecked(bool checked);

    void setToolTip(const ToolTip& toolTip);
    void setToolTipText(const std::string& text, const std::string& replacement = "");

private:
    void doDraw(const DrawParameter& params) override;
    void updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0)) override;
    void layoutUpdated(const sf::Vector2f& screenSize) override;
    void onDrawAdditionalForeground(const DrawParameter& params) override;

    std::unique_ptr<MenuElement> doClone() const override;

    CheckBoxStyle m_style;
    bool m_checked;
    bool m_hover;
    ToolTip m_toolTip;
    bool m_showToolTip;
    Sprite* m_sprite;
};

#endif // CHECKBOX_HPP
