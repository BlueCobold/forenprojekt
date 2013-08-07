#pragma once

#ifndef CHECKBOX_HPP
#define CHECKBOX_HPP

#include "MenuElement.hpp"
#include "CheckboxStyle.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class CheckBox : public MenuElement
{
public:

    CheckBox(int id, CheckBoxStyle style, const sf::Vector2f& position, const sf::Vector2f& offset);

    virtual void update(const sf::RenderWindow& screen) override;
    virtual void draw(const DrawParameter& params) override;
    
    bool getChecked();
    void setChecked(bool checked);

protected:
    
    virtual void onPositionChanged() override;

private:

    bool m_checked;
    sf::Sprite* m_sprite;
    CheckBoxStyle m_style;
};

#endif // CHECKBOX_HPP
