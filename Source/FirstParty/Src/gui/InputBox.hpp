#pragma once

#ifndef INPUTBOX_HPP
#define INPUTBOX_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include "MenuElement.hpp"
#include "LineLabel.hpp"
#include "InputBoxStyle.hpp"
#include "Caret.hpp"
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
             const sf::Vector2f& position,
             const sf::Vector2f& offset,
             const sf::Vector2f size,
             const unsigned int inputLimit,
             const InputBoxStyle& style);

    void update(const sf::RenderWindow& screen, const float time, const sf::Vector2i& mouseOffset = sf::Vector2i(0, 0)) override;

    bool isFinished();

    void setText(const std::string& text);
    std::string getText() const;

    bool isActivatedByMouse() const;

    void disableCaret();

    unsigned int getInputLimit() const;

protected:
    void updateLayout(const sf::Vector2f& screenSize) override;

private:
    std::unique_ptr<MenuElement> doClone() const override;
    void doDraw(const DrawParameter& params) override;

    sf::RectangleShape m_backgroundShade;
    LineLabel m_inputText;
    unsigned int m_inputLimit;
    sf::Vector2f m_size;
    bool m_finished;
    bool m_activated;
    InputBoxStyle m_style;
    std::unordered_map<int, sf::Sprite> m_background;
    float m_scalefactorHorizontal;
    float m_scalefactorVertical;
    Caret m_caret;

    void handleInput();
    void stretchBackground();
    void setBackGroundPosition(const sf::Vector2f& position);

    void setActivatedByMouse(const sf::RenderWindow& screen);
};

#endif // INPUTBOX_HPP
