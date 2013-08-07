#include "ArrowHUD.hpp"
#include "../../resources/ResourceManager.hpp"

#include <cmath>

ArrowHUD::ArrowHUD(ResourceManager& resourceManager):
HUDElement(sf::Vector2f(0.f, 0.f), 0.f),
m_resourceManager(resourceManager),
m_outOfView(false)
{
    m_arrow.setTexture(*m_resourceManager.getTexture("arrowHUD"));
    m_arrow.setTextureRect(sf::IntRect(0, 0, ARROW_WIDTH, ARROW_WIDTH));
    m_arrow.setOrigin(ARROW_WIDTH / 2, ARROW_WIDTH / 2);
}

void ArrowHUD::update(const DrawParameter& params)
{
    sf::Vector2f screenSize = params.getTarget().getView().getSize();
    sf::Vector2f screenCenter = params.getTarget().getView().getCenter();

    float top = screenCenter.y - screenSize.y / 2.f;
    float bottom = screenCenter.y + screenSize.y / 2.f;
    float left = screenCenter.x - screenSize.x / 2.f;
    float right = screenCenter.x + screenSize.x / 2.f;
    /*
    // Version of the same check with a loop. Not much shorter, much harder to understand, far less performant
    m_outOfView = false;
    float vertical[] = { -FLT_MAX, top, bottom, FLT_MAX };
    float horizontal[] = { -FLT_MAX, left, right, FLT_MAX };
    float vpos[] = { top + m_arrowTexture.getTextureRect().height / 2.f,
        m_position.y, bottom - m_arrowTexture.getTextureRect().height / 2.f };
    float hpos[] = { left + m_arrowTexture.getTextureRect().width / 2.f,
        m_position.x, right - m_arrowTexture.getTextureRect().width / 2.f};

    int imgIndex = 0;
    for(int y=0; y<3; y++)
        for(int x=0; x<3; x++)
        {
            if(x==1 && y==1)
                continue;
            if(m_position.x >= horizontal[x] && m_position.x < horizontal[x+1]
            && m_position.y >= vertical[y] && m_position.y < vertical[y+1])
            {
                m_position.x = hpos[x];
                m_position.y = vpos[y];
                m_arrowTexture.setTextureRect(sf::IntRect(imgIndex * 64, 0, 64, 64));
                m_arrowTexture.setPosition(m_position);
                m_outOfView = true;
                return;
            }
            imgIndex++;
        }*/

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

void ArrowHUD::draw(const DrawParameter& params)
{
    if(m_outOfView)
        params.getTarget().draw(m_arrow);
}

void ArrowHUD::setBallCoords(const sf::Vector2f& ballPos)
{
    m_ballPos = ballPos;
}
