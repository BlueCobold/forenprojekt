#include "GrowingCircleTransition.hpp"
#include "../../Utility.hpp"
#include <math.h>

GrowingCircleTransition::GrowingCircleTransition(
    const sf::Texture* sourceTexture,
    const sf::Texture* targetTexture,
    const float duration,
    const sf::Vector2u& size) :
    Transition(sourceTexture, targetTexture, duration, size),
    m_targetSprites(0, 32)
{
    if(targetTexture != nullptr)
    {
        m_RectangleStartY = size.y / 2;
        m_RectangleStartX = size.x / 2;
        m_radius = sqrt(static_cast<float>(m_RectangleStartY * m_RectangleStartY + m_RectangleStartX * m_RectangleStartX));
        m_targetSprites.setTexture(targetTexture);
        m_targetSprites.setTextureRect(sf::IntRect(m_RectangleStartX, m_RectangleStartY, 0, 0));
        m_targetSprites.setPosition(size.x / 2.f, size.y / 2.f);
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("TargetTexture"), "GrowingCircleTransition"));

    if(sourceTexture != nullptr)
    {
        m_sourceSprite.setTexture(*sourceTexture, true);
        m_sourceSprite.setPosition(0, 0);
        m_sourceSprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y));
    }
}

void GrowingCircleTransition::update()
{
    Transition::update();

    float scale = getProgress();

    int currentRadius = static_cast<int>(m_radius * scale);

    m_targetSprites.setPosition(static_cast<float>(m_RectangleStartX - currentRadius),
                                static_cast<float>(m_RectangleStartY - currentRadius));
    m_targetSprites.setTextureRect(sf::IntRect(m_RectangleStartX - currentRadius,
                                               m_RectangleStartY - currentRadius,
                                               currentRadius * 2,
                                               currentRadius * 2));
    
    m_targetSprites.setRadius(static_cast<float>(currentRadius));
}

void GrowingCircleTransition::draw(const DrawParameter& param)
{
    if(getSourceTexture() != nullptr)
        param.getTarget().draw(m_sourceSprite);
    if(getTargetTexture() != nullptr)
        param.getTarget().draw(m_targetSprites);
}
