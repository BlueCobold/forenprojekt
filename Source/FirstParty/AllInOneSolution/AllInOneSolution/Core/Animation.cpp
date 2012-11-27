#include "Animation.hpp"

#include <SFML/Graphics/Rect.hpp>

Animation::Animation(std::unique_ptr<ValueCalculator> calculator,
    const unsigned int frameWidth, const unsigned int frameHeight) :
    m_calculator(std::move(calculator)),
    m_frame(0),
    m_frameWidth(frameWidth),
    m_frameHeight(frameHeight)
{ }

Animation::~Animation()
{ }

void Animation::update()
{
    m_frame = static_cast<int>(m_calculator->calculateValue());
    m_textureRect = sf::IntRect(m_frame * m_frameWidth, 0, m_frameWidth, m_frameHeight);
}

const sf::IntRect& Animation::getTextureRect()
{
    return m_textureRect;
}
