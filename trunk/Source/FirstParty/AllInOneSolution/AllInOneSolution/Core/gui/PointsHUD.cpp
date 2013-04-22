#include "PointsHUD.hpp"
#include "../Utility.hpp"

PointsHUD::PointsHUD(const sf::Vector2f& position,
          const float rotation,
          BitmapFont* bitmapFont,
		  const float horizontalPercentage,
          const float verticalPercentage) :
LabelHUD(position,rotation, bitmapFont, horizontalPercentage, verticalPercentage),
m_pointCounter(0.f,0.f,0.f),
m_elapsedTime(0.f)
{
}

void PointsHUD::update(const DrawParameter& params)
{
    m_pointCounter.update(m_elapsedTime);
    setText(utility::toString<int>(static_cast<int>(m_pointCounter.getCurrentValue())));
    LabelHUD::update(params);
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
