#pragma once

#ifndef INPUTBOX_HPP
#define INPUTBOX_HPP

#include "MenuElement.hpp"
#include "LineLabel.hpp"
#include "InputBoxStyle.hpp"
#include "Caret.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

#include <unordered_map>

class ResourceManager;

class InputBox : public MenuElement
{
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
             const ScreenLocation& position,
             const sf::Vector2f size,
             const unsigned int inputLimit,
             const InputBoxStyle& style);

    bool isFinished();

    void setText(const std::string& text);
    std::string getText() const;

    bool isActivatedByMouse() const;

    void disableCaret();

    unsigned int getInputLimit() const;

private:
    std::unique_ptr<MenuElement> onClone() const override;
    void onDrawElement(const DrawParameter& params) override;
    void updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0)) override;
    void layoutUpdated(const sf::Vector2f& screenSize) override;

    sf::RectangleShape m_backgroundShade;
    LineLabel m_inputText;
    unsigned int m_inputLimit;
    sf::Vector2f m_size;
    bool m_finished;
    bool m_activated;
    InputBoxStyle m_style;
    std::unordered_map<int, Sprite> m_background;
    float m_scalefactorHorizontal;
    float m_scalefactorVertical;
    Caret m_caret;

    void handleInput();
    void stretchBackground();
    void setBackGroundPosition(const sf::Vector2f& position);

    void setActivatedByMouse(const sf::RenderWindow& screen);
};

#endif // INPUTBOX_HPP
