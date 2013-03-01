
#include "VerticalStripesTransition.hpp"

VerticalStripesTransition::VerticalStripesTransition(
    const sf::Texture* sourceTexture,
    const sf::Texture* targetTexture,
    const int stripeCount,
    const float duration) :
    Transition(sourceTexture, targetTexture, duration)
{
    if (targetTexture != nullptr)
    {
        m_stripeWidth = targetTexture->getSize().x / stripeCount;
        for(int i = 0; i < stripeCount; ++i)
        {
            auto sprite = new sf::Sprite(*targetTexture, sf::IntRect(i * m_stripeWidth, 0, 0, targetTexture->getSize().y));
            sprite->setPosition(static_cast<float>(i * m_stripeWidth), 0);
            m_targetSprites.push_back(std::unique_ptr<sf::Sprite>(sprite));
        }
    }
    if (sourceTexture != nullptr)
    {
        m_sourceSprite.setTexture(*sourceTexture, true);
        m_sourceSprite.setPosition(0, 0);
    }
}

void VerticalStripesTransition::update()
{
    Transition::update();

    float scale = getProgress();
    for(auto it = m_targetSprites.begin(); it != m_targetSprites.end(); ++it)
    {
        auto currentRect = (*it)->getTextureRect();
        currentRect.width = static_cast<int>(m_stripeWidth * getProgress());
        (*it)->setTextureRect(currentRect);
    }
}

void VerticalStripesTransition::draw(const DrawParameter& param)
{
    if (getSourceTexture() != nullptr)
        param.getTarget().draw(m_sourceSprite);
    if (getTargetTexture() != nullptr)
    {
        for(auto it = m_targetSprites.begin(); it != m_targetSprites.end(); ++it)
        {
            param.getTarget().draw(**it);
        }
    }
}
