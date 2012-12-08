#include "Animation.hpp"
#include "Utility.hpp" // toDegree, toPixel

#include <SFML/Graphics/Rect.hpp>

#include <cmath>

Animation::Animation(std::unique_ptr<ValueProvider> provider,
    const unsigned int frames,
    const unsigned int frameWidth, const unsigned int frameHeight,
    const bool applyRotation) :
    m_frameProvider(std::move(provider)),
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
    if(m_frameProvider == nullptr)
        m_frame = 0;
    else
        m_frame = static_cast<int>(m_frameProvider->getValue()) % m_frames;
    m_sprite.setTextureRect(getTextureRect());
    sf::Vector2f pos = sf::Vector2f(utility::toPixel(m_externalPos.x), utility::toPixel(m_externalPos.y));
    if(m_xPositionProvider != nullptr)
        pos.x += m_xPositionProvider->getValue();
    if(m_yPositionProvider != nullptr)
        pos.y += m_yPositionProvider->getValue();
    m_sprite.setPosition(pos.x, pos.y);
}

void Animation::bindSubAnimations(std::vector<std::unique_ptr<Animation>>& animations)
{
    m_subAnimations = std::move(animations);
}

void Animation::setPosition(const float x, const float y)
{
    for(auto sub = m_subAnimations.begin(); sub != m_subAnimations.end(); ++sub)
        (*sub)->setPosition(x, y);
    m_externalPos = sf::Vector2f(x, y);
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

void Animation::bindPositionController(std::unique_ptr<ValueProvider> x, std::unique_ptr<ValueProvider> y)
{
    m_xPositionProvider = std::move(x);
    m_yPositionProvider = std::move(y);
}