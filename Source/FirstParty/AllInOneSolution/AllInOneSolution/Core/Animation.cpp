#include "Animation.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/Rect.hpp>

Animation::Animation(std::unique_ptr<ValueCalculator> calculator,
    const unsigned int frameWidth, const unsigned int frameHeight,
    const bool applyRotation) :
    m_calculator(std::move(calculator)),
    m_frame(0),
    m_frameWidth(frameWidth),
    m_frameHeight(frameHeight),
    m_applyRotation(applyRotation)
{ }

Animation::~Animation()
{ }

void Animation::update()
{
    m_frame = static_cast<int>(m_calculator->calculateValue());
    m_sprite.setTextureRect(getTextureRect());
}

void Animation::setPosition(const float x, const float y)
{
    m_sprite.setPosition(x, y);
}

void Animation::setRotation(const float radians)
{
    if(m_applyRotation)
        m_sprite.setRotation(utility::toDegree<float, float>(radians));
}

void Animation::bindTexture(const sf::Texture& texture, const sf::Vector2f& offset)
{
    m_sprite.setTexture(texture);
    m_sprite.setOrigin(offset);
}

const sf::IntRect& Animation::getTextureRect() const
{
    return sf::IntRect(m_frame * m_frameWidth, 0, m_frameWidth, m_frameHeight);
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
}
