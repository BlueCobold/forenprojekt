#include "HorizontalMaskingStripesTransition.hpp"
#include "../../Utility.hpp"

HorizontalMaskingStripesTransition::HorizontalMaskingStripesTransition(
    const sf::Texture* sourceTexture,
    const sf::Texture* targetTexture,
    const int stripeCount,
    const double duration,
    const sf::Vector2u& size) :
    Transition(sourceTexture, targetTexture, duration, size)
{
    if(targetTexture != nullptr)
    {
        m_stripeHeight = size.y / stripeCount;

        if(static_cast<unsigned int>(m_stripeHeight * stripeCount) < size.y)
            m_stripeHeight++;

        m_stripeWidth = size.x;
        for(int i = 0; i < stripeCount; ++i)
        {
            auto sprite = std::unique_ptr<sf::Sprite>(new sf::Sprite(*targetTexture, sf::IntRect(0, i * m_stripeHeight, 0, m_stripeHeight)));
            sprite->setPosition(0, static_cast<float>(i * m_stripeHeight));
            m_targetSprites.push_back(std::move(sprite));
        }
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("@TargetTexture"), "HorizontalMaskingStripesTransition"));

    if(sourceTexture != nullptr)
    {
        m_sourceSprite.setTexture(*sourceTexture, true);
        m_sourceSprite.setPosition(0, 0);
    }
}

void HorizontalMaskingStripesTransition::updated()
{
    auto scale = getProgress();

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

void HorizontalMaskingStripesTransition::doDraw(const DrawParameter& param)
{
    if(getSourceTexture() != nullptr)
        param.getTarget().draw(m_sourceSprite);
    if(getTargetTexture() != nullptr)
        for(auto it = m_targetSprites.begin(); it != m_targetSprites.end(); ++it)
            param.getTarget().draw(**it);
}
