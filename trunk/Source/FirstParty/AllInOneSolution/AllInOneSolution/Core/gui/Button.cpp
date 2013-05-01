#include "Button.hpp"
#include <SFML/Window/Event.hpp>

Button::Button(const sf::Vector2f& position,
               const std::string labelText,
               const std::string font,
               ResourceManager& resourceManager,
               const std::string buttonIdlePicture,
               const sf::Vector2f& textOffset) :
    m_position(position),
    m_resourceManager(resourceManager),
    m_buttonLabel(labelText, sf::Vector2f(0, 0), 0, resourceManager.getBitmapFont(font), LineLabel::Centered)
{
    m_pressed[0] = m_pressed[1] = false;

    m_idleData.m_bound = true;
    m_idleData.m_buttonfont = font;
    m_idleData.m_buttonSprite = buttonIdlePicture;
    m_idleData.m_spriteOffset = sf::Vector2f(0, 0);
    m_idleData.m_textOffset = sf::Vector2f(textOffset.x, textOffset.y);

    m_hoverData.m_bound = m_pressedData.m_bound = false;

    m_buttonSprite.setTexture(*m_resourceManager.getTexture(m_idleData.m_buttonSprite));
    m_buttonSprite.setPosition(m_position);

    float labelPositionX = m_position.x + m_buttonSprite.getTextureRect().width / 2 + textOffset.x;
    float labelPositionY = m_position.y + m_buttonSprite.getTextureRect().height / 2 + textOffset.y - m_buttonLabel.getFontSize() / 2;

    m_buttonLabel.setPosition(labelPositionX, labelPositionY);

    m_size.x = m_buttonSprite.getTextureRect().width;
    m_size.y = m_buttonSprite.getTextureRect().height;
}

void Button::update(const sf::RenderWindow& screen)
{
    sf::FloatRect buttonRect(m_position.x + 2, m_position.y + 2,
                             static_cast<float>(m_buttonSprite.getTextureRect().width - 2),
                             static_cast<float>(m_buttonSprite.getTextureRect().height - 2));

    if(buttonRect.contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(screen))) &&
        sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if(m_pressedData.m_bound)
        {
            m_buttonSprite.setTexture(*m_resourceManager.getTexture(m_pressedData.m_buttonSprite));
            m_buttonLabel.setBitmapFont(m_resourceManager.getBitmapFont(m_pressedData.m_buttonfont));

            m_buttonSprite.setPosition(m_position.x + m_pressedData.m_spriteOffset.x, m_position.y + m_pressedData.m_spriteOffset.y);
            
            float labelPositionX = m_position.x + m_buttonSprite.getTextureRect().width / 2 + m_pressedData.m_textOffset.x;
            float labelPositionY = m_position.y + m_buttonSprite.getTextureRect().height / 2 + m_pressedData.m_textOffset.y - m_buttonLabel.getFontSize() / 2;

            m_buttonLabel.setPosition(labelPositionX, labelPositionY);

            m_pressed[0] = true;
        }
    }
    else if(buttonRect.contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(screen))))
    {
        if(m_hoverData.m_bound)
        {
            m_buttonSprite.setTexture(*m_resourceManager.getTexture(m_hoverData.m_buttonSprite));
            m_buttonLabel.setBitmapFont(m_resourceManager.getBitmapFont(m_hoverData.m_buttonfont));

            m_buttonSprite.setPosition(m_position.x + m_hoverData.m_spriteOffset.x, m_position.y + m_hoverData.m_spriteOffset.y);

            float labelPositionX = m_position.x + m_buttonSprite.getTextureRect().width / 2 + m_hoverData.m_textOffset.x;
            float labelPositionY = m_position.y + m_buttonSprite.getTextureRect().height / 2 + m_hoverData.m_textOffset.y - m_buttonLabel.getFontSize() / 2;

            m_buttonLabel.setPosition(labelPositionX, labelPositionY);

            m_pressed[0] = false;
        }
    }
    else
    {
        m_buttonSprite.setTexture(*m_resourceManager.getTexture(m_idleData.m_buttonSprite));
        m_buttonLabel.setBitmapFont(m_resourceManager.getBitmapFont(m_idleData.m_buttonfont));

        m_buttonSprite.setPosition(m_position.x + m_idleData.m_spriteOffset.x, m_position.y + m_idleData.m_spriteOffset.y);

        float labelPositionX = m_position.x + m_buttonSprite.getTextureRect().width / 2 + m_idleData.m_textOffset.x;
        float labelPositionY = m_position.y + m_buttonSprite.getTextureRect().height / 2 + m_idleData.m_textOffset.y - m_buttonLabel.getFontSize() / 2;

        m_buttonLabel.setPosition(labelPositionX, labelPositionY);

        m_pressed[0] = false;
    }
}

void Button::draw(const DrawParameter& params)
{
    if(m_pressed[0] && m_pressed[1])
        m_pressed[0] = m_pressed[1] = false;

    params.getTarget().draw(m_buttonSprite);
    m_buttonLabel.draw(params);

    if(m_pressed[0])
        m_pressed[1] = !m_pressed[1];
}

void Button::bindHover(std::string buttonSprite,
                       std::string buttonfont,
                       sf::Vector2f spriteOffset,
                       sf::Vector2f textOffset)
{
    m_hoverData.m_bound = true;
    m_hoverData.m_buttonSprite = buttonSprite;
    m_hoverData.m_buttonfont = buttonfont;
    m_hoverData.m_spriteOffset = spriteOffset;
    m_hoverData.m_textOffset = textOffset;
}

void Button::bindPressed(std::string buttonSprite,
                         std::string buttonfont,
                         sf::Vector2f spriteOffset,
                         sf::Vector2f textOffset)
{
    m_pressedData.m_bound = true;
    m_pressedData.m_buttonSprite = buttonSprite;
    m_pressedData.m_buttonfont = buttonfont;
    m_pressedData.m_spriteOffset = spriteOffset;
    m_pressedData.m_textOffset = textOffset;
}

void Button::setPosition(const sf::Vector2f& position)
{
    m_position = position;
}

sf::Vector2i Button::getSize()
{
    return m_size;
}

bool Button::isPressed()
{
    return  m_pressed[0] && m_pressed[1];
}