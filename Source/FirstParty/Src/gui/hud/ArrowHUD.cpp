#include "ArrowHUD.hpp"
#include "../../resources/ResourceManager.hpp"

#include <cmath>

ArrowHUD::ArrowHUD(ResourceManager& resourceManager):
HUDElement(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f)),
m_resourceManager(resourceManager),
m_outOfView(false)
{
    m_arrow.setTexture(*m_resourceManager.getTexture("arrowHUD"));
    m_arrow.setTextureRect(sf::IntRect(0, 0, ARROW_WIDTH, ARROW_WIDTH));
    m_arrow.setOrigin(ARROW_WIDTH / 2, ARROW_WIDTH / 2);
}

void ArrowHUD::updated(const DrawParameter& params)
{
    sf::Vector2f screenSize = params.getTarget().getView().getSize();
    sf::Vector2f screenCenter = params.getTarget().getView().getCenter();

    float top = screenCenter.y - screenSize.y / 2.f;
    float bottom = screenCenter.y + screenSize.y / 2.f;
    float left = screenCenter.x - screenSize.x / 2.f;
    float right = screenCenter.x + screenSize.x / 2.f;

    m_outOfView = true;

    sf::Vector2f position = m_ballPos;
    if(position.y < top)
    {
        if(position.x < left)
        {
            position.x = left + ARROW_WIDTH / 2.f;
            m_arrow.setTextureRect(sf::IntRect(0, 0, ARROW_WIDTH, ARROW_WIDTH));
        }        
        else if(position.x > right)
        {
            position.x = right - ARROW_WIDTH / 2.f;
            m_arrow.setTextureRect(sf::IntRect(2 * ARROW_WIDTH, 0, ARROW_WIDTH, ARROW_WIDTH));
        }
        else
            m_arrow.setTextureRect(sf::IntRect(ARROW_WIDTH, 0, ARROW_WIDTH, ARROW_WIDTH));
        
         position.y = top + ARROW_WIDTH / 2.f;
    }
    else if(position.y > bottom)
    {
        if(position.x < left)
        {
            position.x = left + ARROW_WIDTH / 2.f;
            m_arrow.setTextureRect(sf::IntRect(5 * ARROW_WIDTH, 0, ARROW_WIDTH, ARROW_WIDTH));
        }
        else if(position.x > right)
        {
            position.x = right - ARROW_WIDTH / 2.f;
            m_arrow.setTextureRect(sf::IntRect(7 * ARROW_WIDTH, 0, ARROW_WIDTH, ARROW_WIDTH));
        }
        else
            m_arrow.setTextureRect(sf::IntRect(6 * ARROW_WIDTH, 0, ARROW_WIDTH, ARROW_WIDTH));

        position.y = bottom - ARROW_WIDTH / 2.f;
    }
    else if(position.x < left)
    {
        position.x = left + ARROW_WIDTH / 2.f;
        m_arrow.setTextureRect(sf::IntRect(3 * ARROW_WIDTH, 0, ARROW_WIDTH, ARROW_WIDTH));
    }
    else if(position.x > right)
    {
        position.x = right - ARROW_WIDTH / 2.f;
        m_arrow.setTextureRect(sf::IntRect(4 * ARROW_WIDTH, 0, ARROW_WIDTH, ARROW_WIDTH));
    }
    else
        m_outOfView = false;

    m_arrow.setPosition(position);
}

void ArrowHUD::doDraw(const DrawParameter& params)
{
    if(m_outOfView)
        params.getTarget().draw(m_arrow);
}

void ArrowHUD::setBallCoords(const sf::Vector2f& ballPos)
{
    m_ballPos = ballPos;
}
