#include "Carat.hpp"

#include "../rendering/DrawParameter.hpp"
#include "../resources/ResourceManager.hpp"
#include "../resources/SpriteSheet.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

const std::string SHEET_NAME = "gui_elements";
const std::string SPRITE_NAME = "Caret";

Caret::Caret(float frequency, const sf::Vector2f& offset, const sf::Sprite& sprite) :
    m_blinkTime(1.f / frequency),
    m_nextFullFadeOut(0),
    m_sprite(sprite),
    m_offset(offset),
    m_activated(false)
{
}

void Caret::setPosition(const sf::Vector2f& position)
{
    m_sprite.setPosition(position + m_offset);
}

void Caret::draw(const DrawParameter& params)
{
    if(!m_activated)
        return;

    params.getTarget().draw(m_sprite);
}

void Caret::update(const float time)
{
    if(!m_activated)
        return;

    if(m_nextFullFadeOut < time)
        m_nextFullFadeOut = time + m_blinkTime;

    int alpha = static_cast<int>((m_nextFullFadeOut - time) / m_blinkTime * 255.f);

    m_sprite.setColor(sf::Color(255, 255, 255, alpha));
}

void Caret::disable()
{
    m_activated = false;
}

void Caret::enable()
{
    m_activated = true;
}

bool Caret::isActive()
{
    return m_activated;
}
