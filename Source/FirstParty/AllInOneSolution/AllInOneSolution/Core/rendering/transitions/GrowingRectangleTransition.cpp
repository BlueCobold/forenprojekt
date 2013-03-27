#include "GrowingRectangleTransition.hpp"
#include "../../Utility.hpp"

GrowingRectangleTransition::GrowingRectangleTransition(
    const sf::Texture* sourceTexture,
    const sf::Texture* targetTexture,
    const float duration) :
    Transition(sourceTexture, targetTexture, duration)
{
    if (targetTexture != nullptr)
    {
        m_RectangleStartY = targetTexture->getSize().y / 2;
        m_RectangleStartX = targetTexture->getSize().x / 2;
        m_targetSprites.setTexture(*targetTexture);
        m_targetSprites.setTextureRect(sf::IntRect(m_RectangleStartX, m_RectangleStartY, 0, 0));
        m_targetSprites.setPosition(targetTexture->getSize().x / 2.f, targetTexture->getSize().y / 2.f);
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("TargetTexture"), "GrowingRectangleTransition"));
    if (sourceTexture != nullptr)
    {
        m_sourceSprite.setTexture(*sourceTexture, true);
        m_sourceSprite.setPosition(0, 0);
    }
}

void GrowingRectangleTransition::update()
{
    Transition::update();

    float scale = getProgress();

    m_targetSprites.setPosition(static_cast<float>(m_RectangleStartX - static_cast<int>(m_RectangleStartX * scale)),
                                static_cast<float>(m_RectangleStartY - static_cast<int>(m_RectangleStartY * scale)));
    
    m_targetSprites.setTextureRect(sf::IntRect(m_RectangleStartX - static_cast<int>(m_RectangleStartX * scale),
                                               m_RectangleStartY - static_cast<int>(m_RectangleStartY * scale),
                                               static_cast<int>(m_RectangleStartX * scale * 2.f),
                                               static_cast<int>(m_RectangleStartY * scale * 2.f)));
}

void GrowingRectangleTransition::draw(const DrawParameter& param)
{
    if (getSourceTexture() != nullptr)
        param.getTarget().draw(m_sourceSprite);
    if (getTargetTexture() != nullptr)
        param.getTarget().draw(m_targetSprites);
}