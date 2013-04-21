#include "HorizontalSlidingStripesTransition.hpp"
#include "../../Utility.hpp"

HorizontalSlidingStripesTransition::HorizontalSlidingStripesTransition(
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
        for(int i = 0; i < stripeCount; ++i)
        {
            auto sprite = std::unique_ptr<sf::Sprite>(new sf::Sprite(*targetTexture, sf::IntRect(0, i * m_stripeHeight, 0, m_stripeHeight)));
            sprite->setPosition(0, static_cast<float>(i * m_stripeHeight));
            m_targetSprites.push_back(std::move(sprite));
        }
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("TargetTexture"), "HorizontalSlidingStripesTransition"));

    if(sourceTexture != nullptr)
    {
        for(int i = 0; i < stripeCount; ++i)
        {
            auto sprite = std::unique_ptr<sf::Sprite>(new sf::Sprite(*sourceTexture, sf::IntRect(0, i * m_stripeHeight, m_stripeWidth, m_stripeHeight)));
            sprite->setPosition(0, static_cast<float>(i * m_stripeHeight));
            m_sourceSprites.push_back(std::move(sprite));
        }
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("SourceTexture"), "HorizontalSlidingStripesTransition"));
}

void HorizontalSlidingStripesTransition::update()
{
    Transition::update();

    float scale = getProgress();

    int count = 0;

    int currentWidth = static_cast<int>(m_stripeWidth * scale);

    for(auto it = m_targetSprites.begin(); it != m_targetSprites.end(); ++it)
    {
        if(count % 2 == 0)
        {
            (*it)->setTextureRect(sf::IntRect(m_stripeWidth - currentWidth,
                                  static_cast<int>((*it)->getPosition().y),
                                              currentWidth,
                                              m_stripeHeight));
        }
        else
        {
            (*it)->setTextureRect(sf::IntRect(0,
                                              static_cast<int>((*it)->getPosition().y),
                                              currentWidth,
                                              m_stripeHeight));

            (*it)->setPosition(static_cast<float>(m_stripeWidth - currentWidth),
                               (*it)->getPosition().y);
        }
        count++;
    }

    count = 0;

    for(auto it = m_sourceSprites.begin(); it != m_sourceSprites.end(); ++it)
    {
        if(count % 2 == 0)
        {
            (*it)->setTextureRect(sf::IntRect(0,
                                              static_cast<int>((*it)->getPosition().y),
                                              m_stripeWidth - currentWidth,
                                              m_stripeHeight));

            (*it)->setPosition(static_cast<float>(currentWidth), 
                               (*it)->getPosition().y);
        }
        else
        {
            (*it)->setTextureRect(sf::IntRect(static_cast<int>(m_stripeWidth * scale),
                                              static_cast<int>((*it)->getPosition().y),
                                              m_stripeWidth - currentWidth,
                                              m_stripeHeight));
        }
        count++;
    }
}

void HorizontalSlidingStripesTransition::draw(const DrawParameter& param)
{
    if(getSourceTexture() != nullptr)
        for(auto it = m_sourceSprites.begin(); it != m_sourceSprites.end(); ++it)
            param.getTarget().draw(**it);

    if(getTargetTexture() != nullptr)
        for(auto it = m_targetSprites.begin(); it != m_targetSprites.end(); ++it)
            param.getTarget().draw(**it);
}
