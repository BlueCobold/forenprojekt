
#include "AnimationContainer.hpp"
#include "../animation/CloneHandler.hpp"

AnimationContainer::AnimationContainer(ScreenLocation position,
                                       int id, CloneHandler& cloneHandler) :
    MenuElement(id, MenuElementType::Animation, position),
    m_updatingAni(nullptr),
    m_cloneHandler(cloneHandler),
    m_scale(sf::Vector2f(1.f, 1.f))
{
}

AnimationContainer::AnimationContainer(AnimationContainer&& toMove) :
    MenuElement(toMove.getId(), MenuElementType::Animation, toMove.getPosition()),
    m_updatingAni(toMove.m_updatingAni),
    m_cloneHandler(toMove.m_cloneHandler)
{
}

void AnimationContainer::bindAnimation(std::unique_ptr<Animation> animation)
{
    graphics.bindAnimation(std::move(animation));
}

std::unique_ptr<MenuElement> AnimationContainer::doClone() const
{
    auto other = std::unique_ptr<AnimationContainer>(new AnimationContainer(getPosition(), getId(), m_cloneHandler));
    m_cloneHandler.registerCloneAll(*this, *other.get());
    
    other->setVisibleWhenId(getVisibleWhenId());

    for(auto it = begin(m_variables); it != end(m_variables); ++it)
        other->m_variables[it->first] = it->second;

    other->graphics = graphics;

    m_cloneHandler.unregisterCloneAll(*this);

    return std::move(other);
}

double AnimationContainer::onGetValueOf(const std::string& name) const
{
    auto match = m_variables.find(name);
    if(match == end(m_variables))
    {
        if(m_updatingAni == nullptr)
            throw std::runtime_error(utility::replace(utility::translateKey("@GetVariable"), name));
        return m_updatingAni->getValueOf(name);
    }
    return match->second;
}

void AnimationContainer::onSetValueOf(const std::string& name, const double value)
{
    if(m_updatingAni == nullptr)
        m_variables[name] = value;
    else
    {
        auto match = m_variables.find(name);
        if(match == end(m_variables))
            m_updatingAni->setValueOf(name, value);
        else
            m_variables[name] = value;
    }
}

void AnimationContainer::updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset)
{
    updateCurrentTime(time);
    updateLayout(static_cast<sf::Vector2f>(screen.getSize()));
    auto currentPosition = getCurrentPosition();
    
    graphics.updateAnimations([&](Animation& ani)->bool{
        m_updatingAni = &ani;
        ani.setScale(m_scale);
        ani.setPosition(currentPosition.x, currentPosition.y);
        return true;
    });
    m_updatingAni = nullptr;
}

void AnimationContainer::doDraw(const DrawParameter& param)
{
    if(isVisible())
        graphics.draw(param);
}