#include "VerticalMaskingStripesTransition.hpp"

VerticalMaskingStripesTransition::VerticalMaskingStripesTransition(
    const sf::Texture* sourceTexture,
    const sf::Texture* targetTexture,
    const int stripeCount,
    const float duration) :
    Transition(sourceTexture, targetTexture, duration)
{
    if (targetTexture != nullptr)
    {
        m_stripeHeight = targetTexture->getSize().y;
        m_stripeWidth = targetTexture->getSize().x / stripeCount;
        for(int i = 0; i < stripeCount; ++i)
        {
            auto sprite = new sf::Sprite(*targetTexture, sf::IntRect(i * m_stripeWidth, 0, m_stripeWidth, 0));
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

void VerticalMaskingStripesTransition::update()
{
    Transition::update();

    float scale = getProgress();

    int count = 0;
    for(auto it = m_targetSprites.begin(); it != m_targetSprites.end(); ++it)
    {
        if(count % 2 == 0)
        {
            auto currentRect = (*it)->getTextureRect();
            currentRect.height = static_cast<int>(m_stripeHeight * scale);
            (*it)->setTextureRect(currentRect);
        }
        else
        {
            (*it)->setTextureRect(sf::IntRect(static_cast<int>((*it)->getPosition().x),
                                              m_stripeHeight - static_cast<int>(m_stripeHeight * scale),
                                              m_stripeWidth,
                                              static_cast<int>(m_stripeHeight * scale)));

            (*it)->setPosition((*it)->getPosition().x, 
                               static_cast<float>(m_stripeHeight - static_cast<int>(m_stripeHeight * scale)));
        }
        count++;
    }
}

void VerticalMaskingStripesTransition::draw(const DrawParameter& param)
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
