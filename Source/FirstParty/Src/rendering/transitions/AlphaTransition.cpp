
#include "AlphaTransition.hpp"

AlphaTransition::AlphaTransition(
    const sf::Texture* sourceTexture,
    const sf::Texture* targetTexture,
    const double duration,
    const sf::Vector2u& size) :
    Transition(sourceTexture, targetTexture, duration, size)
{
    if(targetTexture != nullptr)
    {
        m_targetSprite.setTexture(*targetTexture, true);
        m_targetSprite.setPosition(0, 0);
    }
    if(sourceTexture != nullptr)
    {
        m_sourceSprite.setTexture(*sourceTexture, true);
        m_sourceSprite.setPosition(0, 0);
    }
}

void AlphaTransition::updated()
{
    sf::Uint8 alpha = static_cast<sf::Uint8>(255 * getProgress());

    if(getSourceTexture() != nullptr)
        m_sourceSprite.setColor(sf::Color(255, 255, 255, 255 - alpha));
    if(getTargetTexture() != nullptr)
        m_targetSprite.setColor(sf::Color(255, 255, 255, alpha));
}

void AlphaTransition::doDraw(const DrawParameter& param)
{
    if(getSourceTexture() != nullptr)
        param.getTarget().draw(m_sourceSprite);
    if(getTargetTexture() != nullptr)
        param.getTarget().draw(m_targetSprite);
}
