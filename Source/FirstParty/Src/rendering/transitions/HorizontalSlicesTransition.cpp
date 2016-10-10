#include "HorizontalSlicesTransition.hpp"
#include "../../Utility.hpp"

HorizontalSlicesTransition::HorizontalSlicesTransition(
    const sf::Texture* sourceTexture,
    const sf::Texture* targetTexture,
    const int stripeCount,
    const float duration,
    const sf::Vector2u& size) :
    Transition(sourceTexture, targetTexture, duration, size)
{
    if(targetTexture != nullptr)
    {
        m_stripeHeight = size.y;
        m_stripeWidth = size.x / stripeCount;

        if(static_cast<unsigned int>(stripeCount * m_stripeWidth) < size.x)
            m_stripeWidth++;

        m_offset = m_stripeHeight / stripeCount;

        m_timeFactor = static_cast<float>(m_stripeHeight - m_offset + m_stripeHeight) / static_cast<float>(m_offset * stripeCount);

        for(int i = 0; i < stripeCount; ++i)
        {
            auto sprite = std::unique_ptr<sf::Sprite>(new sf::Sprite(*targetTexture, sf::IntRect(i * m_stripeWidth, m_stripeHeight, m_stripeWidth, 0)));
            sprite->setPosition(static_cast<float>(i * m_stripeWidth), 0);
            m_targetSprites.push_back(std::move(sprite));
        }
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("TargetTexture"), "HorizontalSlicesTransition"));

    if(sourceTexture != nullptr)
    {
        m_sourceSprite.setTexture(*sourceTexture, true);
        m_sourceSprite.setPosition(0, 0);
    }
}

void HorizontalSlicesTransition::update()
{
    Transition::update();

    float scale = getProgress() * m_timeFactor;

    int count = 0;

    for(auto it = m_targetSprites.begin(); it != m_targetSprites.end(); ++it)
    {
        (*it)->setPosition(static_cast<float>(m_stripeWidth * count), 
                           static_cast<float>(m_stripeHeight - static_cast<int>(m_stripeHeight * scale) + m_offset * count));

        (*it)->setTextureRect(sf::IntRect(m_stripeWidth * count,
                                          m_stripeHeight - static_cast<int>(m_stripeHeight * scale) + m_offset * count,
                                          m_stripeWidth,
                                          static_cast<int>(m_stripeHeight * scale) + m_offset * count));

        count++;
    }
}

void HorizontalSlicesTransition::doDraw(const DrawParameter& param)
{
    if(getSourceTexture() != nullptr)
        param.getTarget().draw(m_sourceSprite);
    if(getTargetTexture() != nullptr)
        for(auto it = m_targetSprites.begin(); it != m_targetSprites.end(); ++it)
            param.getTarget().draw(**it);
}
