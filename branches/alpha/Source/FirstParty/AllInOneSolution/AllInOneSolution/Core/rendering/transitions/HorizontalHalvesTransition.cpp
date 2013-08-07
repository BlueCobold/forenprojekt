#include "HorizontalHalvesTransition.hpp"
#include "../../Utility.hpp"

HorizontalHalvesTransition::HorizontalHalvesTransition(
    const sf::Texture* sourceTexture,
    const sf::Texture* targetTexture,
    const int stripeCount,
    const float duration,
    const sf::Vector2u& size) :
    Transition(sourceTexture, targetTexture, duration, size)
{
    if(sourceTexture != nullptr)
    {
        m_stripeHeight = size.y / stripeCount;

        if(static_cast<unsigned int>(m_stripeHeight * stripeCount) < size.y)
            m_stripeHeight++;

        m_stripeWidth = size.x;
        for(int i = 0; i < stripeCount; ++i)
        {
            auto sprite = std::unique_ptr<sf::Sprite>(new sf::Sprite(*sourceTexture, sf::IntRect(0, i * m_stripeHeight, m_stripeWidth, m_stripeHeight)));
            sprite->setPosition(0, static_cast<float>(i * m_stripeHeight));
            m_sourceSprites.push_back(std::move(sprite));
        }
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("SourceTexture"), "HorizontalHalvesTransition"));

    if(targetTexture != nullptr)
    {
        m_targetSprite.setTexture(*targetTexture, true);
        m_targetSprite.setPosition(0, 0);
    }
}

void HorizontalHalvesTransition::update()
{
    Transition::update();

    float scale = getProgress();

    int count = 0;

    for(auto it = m_sourceSprites.begin(); it != m_sourceSprites.end(); ++it)
    {
        if(count % 2 == 0)
        {
            (*it)->setTextureRect(sf::IntRect(static_cast<int>(m_stripeWidth * scale), 
                                              static_cast<int>((*it)->getPosition().y),
                                              m_stripeWidth - static_cast<int>(m_stripeWidth * scale),
                                              m_stripeHeight));
        }
        else
        {
            (*it)->setTextureRect(sf::IntRect(0, 
                                              static_cast<int>((*it)->getPosition().y),
                                              m_stripeWidth - static_cast<int>(m_stripeWidth * scale),
                                              m_stripeHeight));

            (*it)->setPosition(m_stripeWidth * scale,
                              (*it)->getPosition().y);
        }
        count++;
    }
}

void HorizontalHalvesTransition::draw(const DrawParameter& param)
{
    if(getTargetTexture() != nullptr)
        param.getTarget().draw(m_targetSprite);

    if(getSourceTexture() != nullptr)
        for(auto it = m_sourceSprites.begin(); it != m_sourceSprites.end(); ++it)
            param.getTarget().draw(**it);
}
