#include "PointsHUD.hpp"
#include "../../Utility.hpp"

PointsHUD::PointsHUD(ResourceManager& resourceManager,
                     const sf::Vector2f& position,
                     const sf::Vector2f& offset,
                     const BitmapFont* bitmapFont) :
    LabelHUD(position, offset, bitmapFont),
    m_pointCounter(0.f,0.f,0.f),
    m_elapsedTime(0.f),
    m_resourceManager(resourceManager)
{
    m_coinTexture.setTexture(*m_resourceManager.getTexture("guiMisc"));
    m_coinTexture.setTextureRect(sf::IntRect(44, 0, 22, 22));
}

void PointsHUD::skipInterpolation()
{
    m_pointCounter.set(m_pointCounter.getTargetValue(), m_pointCounter.getTargetValue(), 0.0f);
}

void PointsHUD::update(const DrawParameter& params)
{
    m_pointCounter.update(m_elapsedTime);
    setText(utility::toString<int>(static_cast<int>(m_pointCounter.getCurrentValue())));
    LabelHUD::update(params);

    m_coinTexture.setPosition(getCurrentPosition().x - 10 - m_coinTexture.getTextureRect().width,
                              getCurrentPosition().y);
}

void PointsHUD::restartAt(const float time)
{
    m_pointCounter.stop();
}

void PointsHUD::setPoints(int points)
{
    if(static_cast<float>(points) != m_pointCounter.getTargetValue())
    {
        m_pointCounter.set(m_pointCounter.getCurrentValue(), static_cast<float>(points), 1.f);
        m_pointCounter.restartAt(m_elapsedTime);
    }
}

void PointsHUD::setTime(float time)
{
    m_elapsedTime = time;
}

void PointsHUD::doDraw(const DrawParameter& params)
{
   params.getTarget().draw(m_coinTexture);
   LabelHUD::doDraw(params);
}
