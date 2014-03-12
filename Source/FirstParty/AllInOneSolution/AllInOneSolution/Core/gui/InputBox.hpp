#pragma once

#ifndef INPUTBOX_HPP
#define INPUTBOX_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include "MenuElement.hpp"
#include "LineLabel.hpp"
#include "InputBoxStyle.hpp"
#include <unordered_map>

class InputBox : public MenuElement
{
private:
    sf::RectangleShape m_backgroundShade;
    unsigned int m_inputLimit;
    unsigned int m_charCounter;
    sf::Vector2f m_size;
    LineLabel m_inputText;
    bool m_finished;
    std::unordered_map<int, sf::Sprite> m_background;
    float m_scalefactorHorizontal;
    float m_scalefactorVertical;

    void handleInput();
    void stretchBackground();
    void setBackGroundPosition(const sf::Vector2f& position);

public:
    enum BackgroundId {TopLeft = 1,
                      TopCenter = 2,
                      TopRight = 3,
                      MiddleLeft = 4,
                      MiddleCenter = 5,
                      MiddleRight = 6,
                      BottomLeft = 7,
                      BottomCenter = 8,
                      BottomRight = 9};

    InputBox(const int id,
             const sf::Vector2f& position,
             const sf::Vector2f& offset,
             const sf::Vector2f size,
             const unsigned int inputLimit,
             const InputBoxStyle& style);

    virtual void draw(const DrawParameter& params) override;
    virtual void update(const sf::RenderWindow& screen, const float time, const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0)) override;

    bool isFinished();

    void onEnter();

    std::string getText() const;

protected:
    virtual void onPositionChanged() override;
};

#endif // INPUTBOX_HPP