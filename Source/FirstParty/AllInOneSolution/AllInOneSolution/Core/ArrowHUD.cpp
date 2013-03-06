#include "ArrowHUD.hpp"
#include "resources\ResourceManager.hpp"

ArrowHUD::ArrowHUD(ResourceManager& resourceManager):
HUDElement(sf::Vector2f(0.f, 0.f), 0.f),
m_resourceManager(resourceManager),
m_outOfView(false)
{
    m_arrowTexture.setTexture(*m_resourceManager.getTexture("arrowHUD"));
    m_arrowTexture.setTextureRect(sf::IntRect(0,0,64,64));
    m_arrowTexture.setOrigin(m_arrowTexture.getTextureRect().width / 2.f, m_arrowTexture.getTextureRect().height / 2.f);
}

void ArrowHUD::update(const DrawParameter& params)
{
    sf::Vector2f screenSize = params.getTarget().getView().getSize();
    sf::Vector2f screenCenter = params.getTarget().getView().getCenter();

    float top = screenCenter.y - screenSize.y / 2.f;
    float bottom = screenCenter.y + screenSize.y / 2.f;
    float left = screenCenter.x - screenSize.x / 2.f;
    float right = screenCenter.x + screenSize.x / 2.f;

    m_outOfView = true;

    if(m_position.x > right && m_position.y < top)
    {
        m_position.x = right - m_arrowTexture.getTextureRect().width / 2.f;
        m_position.y = top + m_arrowTexture.getTextureRect().height / 2.f;
        m_arrowTexture.setTextureRect(sf::IntRect(64,0,64,64));
    }
    else if(m_position.x > right && m_position.y > bottom)
    {
        m_position.x = right - m_arrowTexture.getTextureRect().width / 2.f;
        m_position.y = bottom - m_arrowTexture.getTextureRect().height / 2.f;
        m_arrowTexture.setTextureRect(sf::IntRect(448,0,64,64));
    }
    else if(m_position.x < left && m_position.y < top)
    {
        m_position.x = left + m_arrowTexture.getTextureRect().width / 2.f;
        m_position.y = top + m_arrowTexture.getTextureRect().height / 2.f;
        m_arrowTexture.setTextureRect(sf::IntRect(192,0,64,64));
    }
    else if(m_position.x < left && m_position.y > bottom)
    {
        m_position.x = left + m_arrowTexture.getTextureRect().width / 2.f;
        m_position.y = bottom - m_arrowTexture.getTextureRect().height / 2.f;
        m_arrowTexture.setTextureRect(sf::IntRect(320,0,64,64));
    }
    else if(m_position.y > bottom)
    {
        m_position.y = bottom - m_arrowTexture.getTextureRect().height / 2.f;
        m_arrowTexture.setTextureRect(sf::IntRect(384,0,64,64));
    }
    else if(m_position.y < top)
    {
        m_position.y = top + m_arrowTexture.getTextureRect().height / 2.f;
        m_arrowTexture.setTextureRect(sf::IntRect(128,0,64,64));
    }        
    else if(m_position.x > right)
    {
        m_position.x = right - m_arrowTexture.getTextureRect().width / 2.f;
        m_arrowTexture.setTextureRect(sf::IntRect(0,0,64,64));
    }
    else if(m_position.x < left)
    {
        m_position.x = left + m_arrowTexture.getTextureRect().width / 2.f;
        m_arrowTexture.setTextureRect(sf::IntRect(256,0,64,64));
    }
    else
        m_outOfView = false;

    m_arrowTexture.setPosition(m_position);
}

void ArrowHUD::draw(const DrawParameter& params)
{
    if(m_outOfView)
        params.getTarget().draw(m_arrowTexture);
}

void ArrowHUD::setBallCoords(sf::Vector2f coords)
{
    m_position = coords;
}