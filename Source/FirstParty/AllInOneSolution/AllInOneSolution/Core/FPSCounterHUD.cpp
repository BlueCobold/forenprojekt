#include "FPSCounterHUD.hpp"
#include "resources\ResourceManager.hpp"
#include "Utility.hpp"

FPSCounterHUD::FPSCounterHUD(ResourceManager& resourceManager, const std::string& text, const sf::Vector2f& position, const float rotation, std::string bitmapFont) :
m_resourceManager(resourceManager),
m_label(text,sf::Vector2f(0.f,0.f),rotation,nullptr),
m_bitmapfont(nullptr),
m_position(position)
{
    m_bitmapfont = m_resourceManager.getBitmapFont(bitmapFont);
    m_label.setBitmapFont(m_bitmapfont);
    m_label.setText(utility::toString<int>(m_fpsCounter.getFPS()));
}

void FPSCounterHUD::update(const DrawParameter& params)
{
    sf::Vector2f screenSize = params.getTarget().getView().getSize();
    sf::Vector2f screenCenter = params.getTarget().getView().getCenter();

    m_fpsCounter.update();

    m_label.setPosition(screenCenter.x - screenSize.x / 2 + m_position.x , screenCenter.y - screenSize.y / 2 + m_position.y);
    m_label.setText(utility::toString<int>(m_fpsCounter.getFPS()));
}

void FPSCounterHUD::draw(const DrawParameter& params)
{    
    m_label.draw(params);
}

void FPSCounterHUD::setPosition(sf::Vector2f position)
{
    m_position = position;
}