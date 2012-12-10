#include "Background.hpp"
#include <SFML/Graphics/RenderTarget.hpp>


Background::Background() : m_updatingAni(nullptr)
{
}


Background::~Background()
{
}

void Background::update(const float time)
{
    updateCurrentTime(time);
    for(auto animation = getAnimations().begin(); animation != getAnimations().end(); animation++)
    {
        m_updatingAni = (*animation).get();
        (*animation)->update();
    }
    m_updatingAni = nullptr;
}

float Background::getValueOf(const std::string& name) const
{
    if(m_updatingAni == nullptr)
        throw std::runtime_error("Can't evaluate a variable at this time.");
    return m_updatingAni->getValueOf(name);
}
