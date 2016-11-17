#include "InputBox.hpp"
#include "../Input.hpp"
#include "../Utility.hpp"

InputBox::InputBox(const int id,
                   const sf::Vector2f& position,
                   const sf::Vector2f& offset,
                   const sf::Vector2f size,
                   const unsigned int inputLimit,
                   const InputBoxStyle& style) :
    MenuElement(id, MenuElementType::InputBox, position, offset),
    m_inputText("", position, offset + style.textOffset, 0.f, style.font),
    m_inputLimit(inputLimit),
    m_size(size),
    m_finished(false),
    m_activated(false),
    m_style(style),
    m_background(style.background),
    m_caret(style.caretBlinkFrequency, style.caretOffset, style.caret)
{
    if(m_size.y == 0)
        m_size.y = static_cast<float>(m_inputText.getFontSize());

    m_backgroundShade.setSize(m_size);
    m_backgroundShade.setFillColor(sf::Color(128,128,128,128));

    m_scalefactorHorizontal = m_size.x / static_cast<float>(m_background[TopCenter].getTextureRect().width);
    m_scalefactorVertical = m_size.y / static_cast<float>(m_background[MiddleCenter].getTextureRect().height);

    stretchBackground();

    setBackGroundPosition(position + offset);

    m_caret.enable();

}

std::unique_ptr<MenuElement> InputBox::doClone() const
{
    auto clone = std::unique_ptr<MenuElement>(new InputBox(getId(), getPosition(), getOffset(), m_size, m_inputLimit, m_style));
    clone->setVisibleWhenId(getVisibleWhenId());
    return std::move(clone);
}

void InputBox::updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset)
{
#if defined(IOS) || defined(ANDROID)
    if(!m_caret.isActive())
        m_caret.disable();
#endif    
    setActivatedByMouse(screen);
    handleInput();
    m_caret.update(time);
    m_inputText.update(screen, time, mouseOffset);
    updateLayout(static_cast<sf::Vector2f>(screen.getSize()));
}

void InputBox::doDraw(const DrawParameter& params)
{
    for(auto it = begin(m_background); it != end(m_background); ++it)
        params.getTarget().draw(it->second);

    params.getTarget().draw(m_backgroundShade);
    m_inputText.draw(params);
    m_caret.draw(params);
}

void InputBox::handleInput()
{
    bool shiftKey = utility::Keyboard.isKeyPressed(sf::Keyboard::LShift) || utility::Keyboard.isKeyPressed(sf::Keyboard::RShift);

    if(utility::Keyboard.isKeyDown(sf::Keyboard::Return))
    {
        m_finished = true;
        m_activated = false;
        m_caret.disable();
    }

    if(utility::Keyboard.isKeyDown(sf::Keyboard::BackSpace) && m_inputText.getText().length() > 0)
    {
        std::string text = m_inputText.getText().substr(0, m_inputText.getText().length() - 1);
        m_inputText.setText(text);
    }

    if(m_inputText.getText().length() == m_inputLimit)
        return;

    // handle letters from A to Z
    for(int i = 0; i < 26; ++i)
    {
        if(utility::Keyboard.isKeyDown(static_cast<const sf::Keyboard::Key>(i)))
        {
            if(shiftKey)
                m_inputText.setText(m_inputText.getText() + char(65 + i));
            else
                m_inputText.setText(m_inputText.getText() + char(97 + i));
        }
    }

    // handle numbers from 0 to 9
    for(int i = 0; i < 10; ++i)
    {
        if((utility::Keyboard.isKeyDown(static_cast<const sf::Keyboard::Key>(26 + i)) ||
           utility::Keyboard.isKeyDown(static_cast<const sf::Keyboard::Key>(75 + i))) &&
           !shiftKey)
            m_inputText.setText(m_inputText.getText() + char(48 + i));
    }
}

bool InputBox::isFinished()
{
    return m_finished;
}

std::string InputBox::getText() const
{
    return m_inputText.getText();
}

void InputBox::stretchBackground()
{
    m_background[BottomCenter].setScale(m_scalefactorHorizontal, 1.f);
    m_background[MiddleCenter].setScale(m_scalefactorHorizontal, m_scalefactorVertical);
    m_background[TopCenter].setScale(m_scalefactorHorizontal, 1.f);
    m_background[MiddleLeft].setScale(1.f, m_scalefactorVertical);
    m_background[MiddleRight].setScale(1.f, m_scalefactorVertical);
}

void InputBox::setBackGroundPosition(const sf::Vector2f& position)
{
    m_backgroundShade.setPosition(position);

    m_background[TopLeft].setPosition(position - sf::Vector2f(static_cast<float>(m_background[TopLeft].getTextureRect().width),
                                                              static_cast<float>(m_background[TopLeft].getTextureRect().height)));
    m_background[TopCenter].setPosition(position - sf::Vector2f(0,
                                                                static_cast<float>(m_background[TopLeft].getTextureRect().height)));
    m_background[TopRight].setPosition(position + sf::Vector2f(m_size.x,
                                                               static_cast<float>(-m_background[TopLeft].getTextureRect().height)));
    m_background[MiddleLeft].setPosition(position - sf::Vector2f(static_cast<float>(m_background[MiddleLeft].getTextureRect().width), 0));
    m_background[MiddleCenter].setPosition(position);
    m_background[MiddleRight].setPosition(position + sf::Vector2f(m_size.x, 0));
    m_background[BottomLeft].setPosition(position + sf::Vector2f(static_cast<float>(-m_background[BottomLeft].getTextureRect().width),
                                                                          m_size.y));
    m_background[BottomCenter].setPosition(position + sf::Vector2f(0, m_size.y));
    m_background[BottomRight].setPosition(position + m_size);
}

void InputBox::setText(const std::string& text)
{
    m_finished = false;
    m_inputText.setText(text);
}

void InputBox::setActivatedByMouse(const sf::RenderWindow& screen)
{
    sf::Rect<float> hitBox(m_backgroundShade.getPosition(), m_backgroundShade.getSize());

    if(utility::Mouse.leftButtonDown())
    {
        m_activated = hitBox.contains(static_cast<sf::Vector2f>(getCursorPosition(screen)));
#if defined(IOS) || defined(ANDROID)
        if(m_activated)
            m_caret.enable();
#endif
    }
    else
        m_activated = false;
}

bool InputBox::isActivatedByMouse() const
{
    return m_activated;
}

void InputBox::disableCaret()
{
    m_caret.disable();
}

unsigned int InputBox::getInputLimit() const
{
    return m_inputLimit;
}

void InputBox::layoutUpdated(const sf::Vector2f& screenSize)
{
    auto currentPosition = getCurrentPosition();

    setBackGroundPosition(currentPosition);

    m_inputText.setPosition(getPosition());
    m_caret.setPosition(currentPosition + sf::Vector2f(m_inputText.getWidth(), 0));
}
