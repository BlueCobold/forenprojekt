#include "HorizontalMaskingStripesTransition.hpp"
#include "../../Utility.hpp"

HorizontalMaskingStripesTransition::HorizontalMaskingStripesTransition(
    const sf::Texture* sourceTexture,
    const sf::Texture* targetTexture,
    const int stripeCount,
    const float duration) :
    Transition(sourceTexture, targetTexture, duration)
{
    if(targetTexture != nullptr)
    {
        m_stripeHeight = targetTexture->getSize().y / stripeCount;

        if(static_cast<unsigned int>(m_stripeHeight * stripeCount) < targetTexture->getSize().y)
            m_stripeHeight++;

        m_stripeWidth = targetTexture->getSize().x;
        for(int i = 0; i < stripeCount; ++i)
        {
            auto sprite = new sf::Sprite(*targetTexture, sf::IntRect(0, i * m_stripeHeight, 0, m_stripeHeight));
            sprite->setPosition(0, static_cast<float>(i * m_stripeHeight));
            m_targetSprites.push_back(std::unique_ptr<sf::Sprite>(sprite));
        }
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("TargetTexture"), "HorizontalMaskingStripesTransition"));

    if(sourceTexture != nullptr)
    {
        m_sourceSprite.setTexture(*sourceTexture, true);
        m_sourceSprite.setPosition(0, 0);
    }
}

void HorizontalMaskingStripesTransition::update()
{
    Transition::update();

    float scale = getProgress();

    int count = 0;
    for(auto it = m_targetSprites.begin(); it != m_targetSprites.end(); ++it)
    {
        if(count % 2 == 0)
        {
            auto currentRect = (*it)->getTextureRect();
            currentRect.width = static_cast<int>(m_stripeWidth * scale);
            (*it)->setTextureRect(currentRect);
        }
        else
        {
            (*it)->setTextureRect(sf::IntRect(m_stripeWidth - static_cast<int>(m_stripeWidth * scale), 
                                              static_cast<int>((*it)->getPosition().y),
                                              static_cast<int>(  m_stripeWidth * scale),
                                              m_stripeHeight));

            (*it)->setPosition( static_cast<float>(m_stripeWidth - static_cast<int>(m_stripeWidth * scale)),
                              (*it)->getPosition().y);
        }

        count++;
    }
}

void HorizontalMaskingStripesTransition::draw(const DrawParameter& param)
{
    if(getSourceTexture() != nullptr)
        param.getTarget().draw(m_sourceSprite);
    if(getTargetTexture() != nullptr)
        for(auto it = m_targetSprites.begin(); it != m_targetSprites.end(); ++it)
            param.getTarget().draw(**it);
}