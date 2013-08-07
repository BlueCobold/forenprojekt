#include "VerticalHalvesTransition.hpp"
#include "../../Utility.hpp"

#include <algorithm>

VerticalHalvesTransition::VerticalHalvesTransition(
    const sf::Texture* sourceTexture,
    const sf::Texture* targetTexture,
    const int stripeCount,
    const float duration,
    const sf::Vector2u& size) :
    Transition(sourceTexture, targetTexture, duration, size)
{
    if(sourceTexture != nullptr)
    {
        m_stripeHeight = size.y;
        m_stripeWidth = size.x / stripeCount;

        if(static_cast<unsigned int>(m_stripeWidth * stripeCount) < size.x)
            m_stripeWidth++;

        for(int i = 0; i < stripeCount; ++i)
        {
            auto sprite = std::unique_ptr<sf::Sprite>(new sf::Sprite(*sourceTexture, sf::IntRect(i * m_stripeWidth, 0, m_stripeWidth, m_stripeHeight)));
            sprite->setPosition(static_cast<float>(i * m_stripeWidth), 0);
            m_sourceSprites.push_back(std::move(sprite));
        }
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("SourceTexture"), "HorizontalMaskingStripesTransition"));

    if(targetTexture != nullptr)
    {
        m_targetSprite.setTexture(*targetTexture, true);
        m_targetSprite.setPosition(0, 0);
    }
}

void VerticalHalvesTransition::update()
{
    Transition::update();

    float scale = getProgress();

    int count = 0;
    for(auto it = m_sourceSprites.begin(); it != m_sourceSprites.end(); ++it)
    {
        if(count % 2 == 0)
        {
            (*it)->setTextureRect(sf::IntRect(static_cast<int>((*it)->getPosition().x),
                                              static_cast<int>(m_stripeHeight * scale),                                          
                                              m_stripeWidth,
                                              m_stripeHeight - static_cast<int>(m_stripeHeight * scale)));
        }
        else
        {
            (*it)->setTextureRect(sf::IntRect(static_cast<int>((*it)->getPosition().x), 
                                              0,
                                              m_stripeWidth,
                                              m_stripeHeight - static_cast<int>(m_stripeHeight * scale)));

            (*it)->setPosition((*it)->getPosition().x,
                               m_stripeHeight * scale);
        }
        count++;
    }
}

void VerticalHalvesTransition::draw(const DrawParameter& param)
{
    if(getTargetTexture() != nullptr)
        param.getTarget().draw(m_targetSprite);

    if(getSourceTexture() != nullptr)
        for(auto it = m_sourceSprites.begin(); it != m_sourceSprites.end(); ++it)
            param.getTarget().draw(**it);
}
