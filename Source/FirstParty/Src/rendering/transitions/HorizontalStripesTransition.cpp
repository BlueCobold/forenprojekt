#include "HorizontalStripesTransition.hpp"
#include "../../Utility.hpp"

HorizontalStripesTransition::HorizontalStripesTransition(
    const sf::Texture* sourceTexture,
    const sf::Texture* targetTexture,
    const int stripeCount,
    const float duration,
    const sf::Vector2u& size) :
    Transition(sourceTexture, targetTexture, duration, size)
{
    if (targetTexture != nullptr)
    {
        m_stripeHeight = size.y / stripeCount;
        for(int i = 0; i < stripeCount; ++i)
        {
            auto sprite = std::unique_ptr<sf::Sprite>(new sf::Sprite(*targetTexture, sf::IntRect(0, i * m_stripeHeight, size.x, 0)));
            sprite->setPosition(0, static_cast<float>(i * m_stripeHeight));
            m_targetSprites.push_back(std::move(sprite));
        }
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("TargetTexture"), "HorizontalStripesTransition"));
    if (sourceTexture != nullptr)
    {
        m_sourceSprite.setTexture(*sourceTexture, true);
        m_sourceSprite.setPosition(0, 0);
    }
}

void HorizontalStripesTransition::update()
{
    Transition::update();

    for(auto it = m_targetSprites.begin(); it != m_targetSprites.end(); ++it)
    {
        auto currentRect = (*it)->getTextureRect();
        currentRect.height = static_cast<int>(m_stripeHeight * getProgress());
        (*it)->setTextureRect(currentRect);
    }
}

void HorizontalStripesTransition::draw(const DrawParameter& param)
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
