#pragma once

#ifndef CHECKBOX_HPP
#define CHECKBOX_HPP

#include "../rendering/Drawable.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "CheckboxStyle.hpp"

class CheckBox : public Drawable
{
public:
    CheckBox(int id, CheckBoxStyle style, const sf::Vector2f& position, const sf::Vector2f& offset);

    virtual void update(const sf::RenderWindow& screen);

    virtual void draw(const DrawParameter& params) override;

    int getId() const;

    void setPosition(sf::Vector2f position);

    bool getChecked();

    void setChecked(bool checked);

private:
    bool m_checked;

    int m_id;

    sf::Vector2f m_position;

    sf::Vector2f m_offset;

    sf::Sprite* m_sprite;

    CheckBoxStyle m_style;
};

#endif // CHECKBOX_HPP
