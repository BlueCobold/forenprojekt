#include "VerticalSlicesTransition.hpp"
#include "../../Utility.hpp"

VerticalSlicesTransition::VerticalSlicesTransition(
    const sf::Texture* sourceTexture,
    const sf::Texture* targetTexture,
    const int stripeCount,
    const float duration,
    const sf::Vector2u& size) :
    Transition(sourceTexture, targetTexture, duration, size)
{
    if(targetTexture != nullptr)
    {
        m_stripeHeight = size.y / stripeCount;

        if(static_cast<unsigned int>(m_stripeHeight * stripeCount) < size.y)
            m_stripeHeight++;

        m_stripeWidth = size.x;

        m_offset = m_stripeWidth / stripeCount;

        m_timeFactor = static_cast<float>(m_stripeWidth - m_offset + m_stripeWidth) / static_cast<float>(m_offset * stripeCount);

        for(int i = 0; i < stripeCount; ++i)
        {
            auto sprite = std::unique_ptr<sf::Sprite>(new sf::Sprite(*targetTexture, sf::IntRect(m_stripeWidth, i * m_stripeHeight, 0, m_stripeHeight)));
            sprite->setPosition(0, static_cast<float>(i * m_stripeHeight));
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

void VerticalSlicesTransition::update()
{
    Transition::update();

    float scale = getProgress() * m_timeFactor;

    int count = 0;

    for(auto it = m_targetSprites.begin(); it != m_targetSprites.end(); ++it)
    {
        (*it)->setPosition(static_cast<float>(m_stripeWidth - static_cast<int>(m_stripeWidth * scale) + m_offset * count),
                           static_cast<float>(m_stripeHeight * count));

        (*it)->setTextureRect(sf::IntRect(m_stripeWidth - static_cast<int>(m_stripeWidth * scale) + m_offset * count,
                                          m_stripeHeight * count,
                                          static_cast<int>(m_stripeWidth * scale) + m_offset * count,
                                          m_stripeHeight));
        count++;
    }
}

void VerticalSlicesTransition::draw(const DrawParameter& param)
{
    if(getSourceTexture() != nullptr)
        param.getTarget().draw(m_sourceSprite);
    if(getTargetTexture() != nullptr)
        for(auto it = m_targetSprites.begin(); it != m_targetSprites.end(); ++it)
            param.getTarget().draw(**it);
}
