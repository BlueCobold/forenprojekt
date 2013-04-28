#pragma once

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../rendering/Drawable.hpp"
#include "LineLabel.hpp"
#include "../resources/ResourceManager.hpp"

class Button
{
public:

	static const float Left;
	static const float Right;
	static const float Center;
	static const float Top;
	static const float Middle;
	static const float Bottom;

    struct ButtonData 
    {
        std::string m_buttonSprite;
        std::string m_buttonfont;
        sf::Vector2f m_spriteOffset;
        sf::Vector2f m_textOffset;
        bool m_bound;
    };

private:

    sf::Vector2f m_position;
    sf::Vector2i m_size;

    float m_horizontalPercentage;

    float m_verticalPercentage;

    LineLabel m_buttonLabel;

    sf::Sprite m_buttonSprite;

    ButtonData m_idleData;
    ButtonData m_hoverData;
    ButtonData m_pressedData;

    ResourceManager& m_resourceManager;

    sf::RenderWindow& m_window;

    bool m_pressed[2];

public:

    Button(const sf::Vector2f& position,
           const std::string labelText,
           const std::string font,
           ResourceManager& resourceManager,
           sf::RenderWindow& window,
           const std::string buttonIdlePicture,
           const sf::Vector2f& textOffset = sf::Vector2f(0, 0),
		   const float horizontalPercentage = Button::Left,
           const float verticalPercentage = Button::Top);

    virtual ~Button()
    { }

    virtual void update();

    virtual void draw(const DrawParameter& params);

    void bindHover(std::string buttonSprite,
                   std::string buttonfont,
                   sf::Vector2f spriteOffset = sf::Vector2f(0, 0),
                   sf::Vector2f textOffset = sf::Vector2f(0, 0));

    void bindPressed(std::string buttonSprite,
                     std::string buttonfont,
                     sf::Vector2f spriteOffset = sf::Vector2f(0, 0),
                     sf::Vector2f textOffset = sf::Vector2f(0, 0));

    void setPosition(const sf::Vector2f& position, const float horizontalPercentage = Button::Left, const float verticalPercentage = Button::Top);

    sf::Vector2i getSize();

    bool isPressed();
};

#endif // BUTTON_HPP