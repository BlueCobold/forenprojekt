#include "Animation.hpp"
#include "Utility.hpp" // toDegree, toPixel

#include <SFML/Graphics/Rect.hpp>

#include <cmath>

Animation::Animation(std::unique_ptr<ValueProvider> provider,
    const unsigned int frames,
    const unsigned int frameWidth, const unsigned int frameHeight,
    const bool applyRotation) :
    m_provider(std::move(provider)),
    m_frames(frames),
    m_frame(0),
    m_frameWidth(frameWidth),
    m_frameHeight(frameHeight),
    m_applyRotation(applyRotation)
{ }

Animation::~Animation()
{ }

void Animation::update()
{
    for(auto sub = m_subAnimations.begin(); sub != m_subAnimations.end(); ++sub)
        (*sub)->update();
    if(m_provider == nullptr)
        m_frame = 0;
    else
        m_frame = static_cast<int>(m_provider->getValue()) % m_frames;
    m_sprite.setTextureRect(getTextureRect());
}

void Animation::bindSubAnimations(std::vector<std::unique_ptr<Animation>>& animations)
{
    m_subAnimations = std::move(animations);
}

void Animation::setPosition(const float x, const float y)
{
    for(auto sub = m_subAnimations.begin(); sub != m_subAnimations.end(); ++sub)
        (*sub)->setPosition(x, y);
    m_sprite.setPosition(utility::toPixel<float>(x), utility::toPixel<float>(y));
}

void Animation::setRotation(const float radians)
{
    for(auto sub = m_subAnimations.begin(); sub != m_subAnimations.end(); ++sub)
        (*sub)->setRotation(radians);
    if(m_applyRotation)
        m_sprite.setRotation(utility::toDegree<float, float>(radians));
}

void Animation::bindTexture(const sf::Texture& texture, const sf::Vector2f& offset, const sf::Vector2f& sourceOffset)
{
    m_sourceOffset = sourceOffset;
    m_sprite.setTexture(texture);
    m_sprite.setOrigin(offset);
}

const sf::IntRect Animation::getTextureRect() const
{
    return sf::IntRect(static_cast<int>(m_frame * m_frameWidth + m_sourceOffset.x), static_cast<int>(m_sourceOffset.y), m_frameWidth, m_frameHeight);
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
    for(auto sub = m_subAnimations.begin(); sub != m_subAnimations.end(); ++sub)
        (*sub)->draw(target, states);
}
