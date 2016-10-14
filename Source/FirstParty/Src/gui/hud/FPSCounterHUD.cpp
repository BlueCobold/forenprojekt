#include "FPSCounterHUD.hpp"
#include "../../resources/ResourceManager.hpp"
#include "../../Utility.hpp"

FPSCounterHUD::FPSCounterHUD(const sf::Vector2f& position,
                             const sf::Vector2f& offset,
                             const BitmapFont* bitmapFont) :
    LabelHUD(position, offset, bitmapFont) 
{
    setText(utility::toString<int>(m_fpsCounter.getFPS()));
}

void FPSCounterHUD::updated(const DrawParameter& params)
{
    LabelHUD::updated(params);

    m_fpsCounter.update();

    setText(utility::toString<int>(m_fpsCounter.getFPS()));
}
