#include "FPSCounterHUD.hpp"
#include "../../resources/ResourceManager.hpp"
#include "../../Utility.hpp"

FPSCounterHUD::FPSCounterHUD(const sf::Vector2f& position,
                             const sf::Vector2f& offset,
                             const BitmapFont* bitmapFont) :
    LabelHUD(position, offset, bitmapFont) 
{
    m_label.setText(utility::toString<int>(m_fpsCounter.getFPS()));
}

void FPSCounterHUD::update(const DrawParameter& params)
{
    LabelHUD::update(params);

    m_fpsCounter.update();

    m_label.setText(utility::toString<int>(m_fpsCounter.getFPS()));
}
