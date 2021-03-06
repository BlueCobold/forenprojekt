#include "TimeHUD.hpp"
#include "../../resources/ResourceManager.hpp"
#include "../../Utility.hpp"

TimeHUD::TimeHUD(ResourceManager& resourceManager,
                 const sf::Vector2f& position,
                 const sf::Vector2f& offset,
                 const BitmapFont* bitmapFont) :
    LabelHUD(position, offset, bitmapFont),
    m_seconds("00"),
    m_minutes("00"),
    m_totalSeconds(0),
    m_resourceManager(resourceManager)
{
    setAlignment(LineLabel::Left);
    setText(m_minutes + " " + m_seconds);
    m_clock.setTexture(*m_resourceManager.getTexture("guiMisc"));
    m_clock.setTextureRect(sf::IntRect(22, 0, 22, 22));
}

void TimeHUD::setTime(int totalSeconds)
{
    if(m_totalSeconds != totalSeconds)
    {
        int seconds = totalSeconds % 60;
        int minutes = totalSeconds / 60;

        if(seconds < 10)
            m_seconds = "0" + utility::toString(seconds);
        else
            m_seconds = utility::toString(seconds);
        if(minutes < 10)
            m_minutes = "0" + utility::toString(minutes);
        else
            m_minutes = utility::toString(minutes);
    }
}

void TimeHUD::updated(const DrawParameter& params)
{
    LabelHUD::updated(params);

    setText(m_minutes + ":" + m_seconds);

    m_clock.setPosition(getCurrentPosition().x - 10 - m_clock.getTextureRect().width,
                        getCurrentPosition().y);
}

void TimeHUD::doDraw(const DrawParameter& params)
{
   params.getTarget().draw(m_clock);
   LabelHUD::doDraw(params);
}
