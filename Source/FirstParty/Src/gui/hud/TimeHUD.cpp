#include "TimeHUD.hpp"
#include "../../resources/ResourceManager.hpp"
#include "../../Utility.hpp"

TimeHUD::TimeHUD(ResourceManager& resourceManager, 
                 const sf::Vector2f& position, 
                 const float rotation, 
                 BitmapFont* bitmapFont,
                 const float hReference,
                 const float vReference) :
    LabelHUD(position, rotation, bitmapFont, hReference, vReference),
    m_seconds("00"),
    m_minutes("00"),
    m_totalSeconds(0),
    m_resourceManager(resourceManager)
{
    m_label.setAlignment(LineLabel::Left);
    m_label.setText(m_minutes + " " + m_seconds);
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

void TimeHUD::update(const DrawParameter& params)
{
    LabelHUD::update(params);

    m_label.setText(m_minutes + ":" + m_seconds);

    m_clock.setPosition(m_label.getPosition().x - 10 - m_clock.getTextureRect().width,
        m_label.getPosition().y);
}

void TimeHUD::draw(const DrawParameter& params)
{
   params.getTarget().draw(m_clock);
   LabelHUD::draw(params);
}
