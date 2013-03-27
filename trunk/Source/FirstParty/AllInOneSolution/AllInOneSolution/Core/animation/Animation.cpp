#include "Animation.hpp"
#include "../Utility.hpp" // toDegree, toPixel

#include <SFML/Graphics/Rect.hpp>

#include <cmath>
#include <SFML/System/Clock.hpp>

Animation::Animation(std::unique_ptr<ValueProvider> provider,
    const unsigned int frames,
    const unsigned int frameWidth, const unsigned int frameHeight,
    const bool applyRotation,
    const sf::Vector2f& origin,
    const sf::Vector2f& drawOffset,
    const bool horizontal) :
    m_frameProvider(std::move(provider)),
    m_frames(frames),
    m_frame(0),
    m_frameWidth(frameWidth),
    m_frameHeight(frameHeight),
    m_applyRotation(applyRotation),
    m_drawOffset(drawOffset),
    m_externalRotation(0.f),
    m_horizontal(horizontal)
{
    m_sprite.setOrigin(origin);
}

Animation::~Animation()
{ }

void Animation::update()
{
    sf::Sprite s(m_sprite);
    m_sprite = s;

    if(m_frameProvider == nullptr)
        m_frame = 0;
    else
        m_frame = static_cast<int>(m_frameProvider->getValue()) % m_frames;
    m_sprite.setTextureRect(getTextureRect());
    
    float rotation = m_externalRotation;
    if(m_rotationProvider != nullptr)
        rotation += m_rotationProvider->getValue();
    m_sprite.setRotation(rotation);

    updatePosition();
    
    float scaleX = 1, scaleY = 1;
    if(m_xScaleProvider != nullptr)
        scaleX = m_xScaleProvider->getValue();
    if(m_yScaleProvider != nullptr)
        scaleY = m_yScaleProvider->getValue();
    if(m_xScaleProvider != nullptr || m_yScaleProvider != nullptr)
    m_sprite.setScale(scaleX, scaleY);

    sf::Color color(255, 255, 255);
    if(m_colorProviders[Red] != nullptr)
        color.r = static_cast<sf::Uint8>(255*m_colorProviders[Red]->getValue());
    if(m_colorProviders[Green] != nullptr)
        color.g = static_cast<sf::Uint8>(255*m_colorProviders[Green]->getValue());
    if(m_colorProviders[Blue] != nullptr)
        color.b = static_cast<sf::Uint8>(255*m_colorProviders[Blue]->getValue());
    if(m_colorProviders[Alpha] != nullptr)
        color.a = static_cast<sf::Uint8>(255*m_colorProviders[Alpha]->getValue());
    m_sprite.setColor(color);
}

void Animation::setPosition(const float x, const float y)
{
    m_externalPosition = sf::Vector2f(x, y);
}

void Animation::updatePosition()
{
    sf::Vector2f pos = sf::Vector2f(m_externalPosition.x, m_externalPosition.y) + m_drawOffset;
    if(m_xPositionProvider != nullptr)
        pos.x += m_xPositionProvider->getValue();
    if(m_yPositionProvider != nullptr)
        pos.y += m_yPositionProvider->getValue();
    m_sprite.setPosition(pos.x, pos.y);
}

void Animation::setRotation(const float radians)
{
    if(m_applyRotation)
        m_externalRotation = utility::toDegree<float, float>(radians);
}

void Animation::bindTexture(const sf::Texture& texture, const sf::Vector2f& sourceOffset)
{
    m_sourceOffset = sourceOffset;
    m_sprite.setTexture(texture);
}

const sf::IntRect Animation::getTextureRect() const
{
    if(m_horizontal)
        return sf::IntRect(static_cast<int>(m_frame * m_frameWidth + m_sourceOffset.x), static_cast<int>(m_sourceOffset.y), m_frameWidth, m_frameHeight);
    else
        return sf::IntRect(static_cast<int>(m_sourceOffset.x), static_cast<int>(m_frame * m_frameHeight + m_sourceOffset.y), m_frameWidth, m_frameHeight);
}

void Animation::draw(const DrawParameter& param)
{
    if(param.getScreenRect().intersects(m_sprite.getGlobalBounds()))
        param.getTarget().draw(m_sprite);
}

void Animation::bindPositionController(std::unique_ptr<ValueProvider> x, std::unique_ptr<ValueProvider> y)
{
    m_xPositionProvider = std::move(x);
    m_yPositionProvider = std::move(y);
}

void Animation::bindRotationController(std::unique_ptr<ValueProvider> provider)
{
    m_rotationProvider = std::move(provider);
}

void Animation::bindScaleController(std::unique_ptr<ValueProvider> x, std::unique_ptr<ValueProvider> y)
{
    m_xScaleProvider = std::move(x);
    m_yScaleProvider = std::move(y);
}

void Animation::bindColorController(std::unique_ptr<ValueProvider> red,
    std::unique_ptr<ValueProvider> green,
    std::unique_ptr<ValueProvider> blue,
    std::unique_ptr<ValueProvider> alpha)
{
    m_colorProviders[Red] = std::move(red);
    m_colorProviders[Green] = std::move(green);
    m_colorProviders[Blue] = std::move(blue);
    m_colorProviders[Alpha] = std::move(alpha);
}
