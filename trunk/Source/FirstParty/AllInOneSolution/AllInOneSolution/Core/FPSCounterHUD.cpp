#include "FPSCounterHUD.hpp"
#include "resources\ResourceManager.hpp"
#include "Utility.hpp"

FPSCounterHUD::FPSCounterHUD(ResourceManager& resourceManager, const sf::Vector2f& position, const float rotation, std::string bitmapFont) :
HUDElement(resourceManager,position,rotation,bitmapFont) 
{
    m_label.setText(utility::toString<int>(m_fpsCounter.getFPS()));
}

void FPSCounterHUD::update(const DrawParameter& params)
{
    HUDElement::update(params);

    m_fpsCounter.update();

    m_label.setText(utility::toString<int>(m_fpsCounter.getFPS()));
}