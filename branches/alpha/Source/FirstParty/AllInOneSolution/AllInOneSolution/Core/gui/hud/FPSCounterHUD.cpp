#include "FPSCounterHUD.hpp"
#include "../../resources/ResourceManager.hpp"
#include "../../Utility.hpp"

FPSCounterHUD::FPSCounterHUD(const sf::Vector2f& position, const float rotation, BitmapFont* bitmapFont, float hReference, float vReference) :
    LabelHUD(position, rotation, bitmapFont, hReference, vReference) 
{
    m_label.setText(utility::toString<int>(m_fpsCounter.getFPS()));
}

void FPSCounterHUD::update(const DrawParameter& params)
{
    LabelHUD::update(params);

    m_fpsCounter.update();

    m_label.setText(utility::toString<int>(m_fpsCounter.getFPS()));
}
