#include "VerticalCompressTransition.hpp"
#include "../../Utility.hpp"

VerticalCompressTransition::VerticalCompressTransition(
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

        if(static_cast<unsigned int>(m_stripeWidth * stripeCount) < size.x)
            m_stripeWidth++;

        for(int i = 0; i < stripeCount; ++i)
        {
            auto sprite = std::unique_ptr<sf::Sprite>(new sf::Sprite(*targetTexture, sf::IntRect(i * m_stripeWidth, 0, m_stripeWidth, 0)));
            sprite->setPosition(static_cast<float>(i * m_stripeWidth), 0);
            m_targetSprites.push_back(std::move(sprite));
        }
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("TargetTexture"), "VerticalCompressTransition"));

    if(sourceTexture != nullptr)
    {
        for(int i = 0; i < stripeCount; ++i)
        {
            auto sprite = std::unique_ptr<sf::Sprite>(new sf::Sprite(*sourceTexture, sf::IntRect(i * m_stripeWidth, 0, m_stripeWidth, m_stripeHeight)));
            sprite->setPosition(static_cast<float>(i * m_stripeWidth), 0);
            m_sourceSprites.push_back(std::move(sprite));
        }
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("SourceTexture"), "VerticalCompressTransition"));
}

void VerticalCompressTransition::update()
{
    Transition::update();

    float scale = getProgress();

    int count = 0;

    int currentHeight = static_cast<int>(m_stripeHeight * scale);

    for(auto it = m_targetSprites.begin(); it != m_targetSprites.end(); ++it)
    {
        if(count % 2 == 0)
        {
            (*it)->setTextureRect(sf::IntRect(static_cast<int>((*it)->getPosition().x),
                                              m_stripeHeight - currentHeight,
                                              m_stripeWidth,
                                              currentHeight));
        }
        else
        {
            (*it)->setTextureRect(sf::IntRect(static_cast<int>((*it)->getPosition().x),
                                              0,
                                              m_stripeWidth,
                                              currentHeight));

            (*it)->setPosition((*it)->getPosition().x, 
                               static_cast<float>(m_stripeHeight - currentHeight));
        }
        count++;
    }
    
    count = 0;

    for(auto it = m_sourceSprites.begin(); it != m_sourceSprites.end(); ++it)
    {
        if(count % 2 == 0)
        {
            (*it)->setScale(1, 1 - scale);

            (*it)->setPosition((*it)->getPosition().x, 
                               static_cast<float>(currentHeight));
            
        }
        else
            (*it)->setScale(1, 1 - scale);

        count++;
    }
}

void VerticalCompressTransition::draw(const DrawParameter& param)
{
    if(getSourceTexture() != nullptr)
        for(auto it = m_sourceSprites.begin(); it != m_sourceSprites.end(); ++it)
            param.getTarget().draw(**it);

    if(getTargetTexture() != nullptr)
        for(auto it = m_targetSprites.begin(); it != m_targetSprites.end(); ++it)
            param.getTarget().draw(**it);
}
