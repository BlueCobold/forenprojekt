#pragma once

#ifndef CHECKBOX_HPP
#define CHECKBOX_HPP

#include "CheckboxStyle.hpp"
#include "MenuElement.hpp"
#include "ToolTip.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class CheckBox : public MenuElement
{
public:

    CheckBox(int id, CheckBoxStyle style, const sf::Vector2f& position, const sf::Vector2f& offset);

    virtual void update(const sf::RenderWindow& screen, const float time, const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0)) override;
    virtual void draw(const DrawParameter& params) override;
    virtual void drawAdditionalForeground(const DrawParameter& params) override;
    
    bool getChecked();
    void setChecked(bool checked);
    
    void setToolTip(const ToolTip& toolTip);
    void setToolTipText(const std::string& text);

protected:
    
    virtual void onPositionChanged() override;

private:
    
    ToolTip m_toolTip;
    bool m_showToolTip;

    bool m_checked;
    bool m_hover;
    sf::Sprite* m_sprite;
    CheckBoxStyle m_style;
};

#endif // CHECKBOX_HPP
