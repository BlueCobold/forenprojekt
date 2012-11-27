#include "Background.hpp"
#include <SFML/Graphics/RenderTarget.hpp>


Background::Background()
{
}


Background::~Background()
{
}

void Background::update(const float time)
{
    updateCurrentTime(time);
    getAnimation()->update();
}