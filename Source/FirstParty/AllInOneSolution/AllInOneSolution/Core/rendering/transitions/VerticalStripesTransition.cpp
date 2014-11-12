#include "VerticalStripesTransition.hpp"
#include "../../Utility.hpp"

VerticalStripesTransition::VerticalStripesTransition(
    const sf::Texture* sourceTexture,
    const sf::Texture* targetTexture,
    const int stripeCount,
    const float duration,
    const sf::Vector2u& size) :
    Transition(sourceTexture, targetTexture, duration, size)
{
    if(targetTexture != nullptr)
    {
        m_stripeWidth = size.x / stripeCount;

        if(static_cast<unsigned int>(m_stripeWidth * stripeCount) < size.x)
            m_stripeWidth++;

        for(int i = 0; i < stripeCount; ++i)
        {
            auto sprite = std::unique_ptr<sf::Sprite>(new sf::Sprite(*targetTexture, sf::IntRect(i * m_stripeWidth, 0, 0, size.y)));
            sprite->setPosition(static_cast<float>(i * m_stripeWidth), 0);
            m_targetSprites.push_back(std::move(sprite));
        }
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("TargetTexture"), "VerticalStripesTransition"));
    
    if(sourceTexture != nullptr)
    {
        m_sourceSprite.setTexture(*sourceTexture, true);
        m_sourceSprite.setPosition(0, 0);
    }
}

void VerticalStripesTransition::update()
{
   Transition::update();

    //float scale = getProgress();

   for(auto it = m_targetSprites.begin(); it != m_targetSprites.end(); ++it)
   {
        auto currentRect = (*it)->getTextureRect();
        currentRect.width = static_cast<int>(m_stripeWidth * getProgress());
        (*it)->setTextureRect(currentRect);
    }
}

void VerticalStripesTransition::draw(const DrawParameter& param)
{
    if(getSourceTexture() != nullptr)
        param.getTarget().draw(m_sourceSprite);
    if(getTargetTexture() != nullptr)
        for(auto it = m_targetSprites.begin(); it != m_targetSprites.end(); ++it)
            param.getTarget().draw(**it);
}
