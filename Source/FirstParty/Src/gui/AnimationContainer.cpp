
#include "AnimationContainer.hpp"
#include "../animation/CloneHandler.hpp"

AnimationContainer::AnimationContainer(const sf::Vector2f& position,
                                       const sf::Vector2f& offset,
                                       int id, CloneHandler& cloneHandler) :
    MenuElement(id, MenuElementType::Animation, position, offset),
    m_updatingAni(nullptr),
    m_cloneHandler(cloneHandler)
{
}

AnimationContainer::AnimationContainer(AnimationContainer&& toMove) :
    MenuElement(toMove.getId(), MenuElementType::Animation, toMove.getPosition(), toMove.getOffset()),
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
    auto other = std::unique_ptr<AnimationContainer>(new AnimationContainer(getPosition(), getOffset(), getId(), m_cloneHandler));
    m_cloneHandler.registerCloneAll(*this, *other.get());
    
    other->setVisibleWhenId(getVisibleWhenId());

    for(auto it = begin(m_variables); it != end(m_variables); ++it)
        other->m_variables[it->first] = it->second;

    other->graphics = graphics;

    m_cloneHandler.unregisterCloneAll(*this);

    return std::move(other);
}

float AnimationContainer::onGetValueOf(const std::string& name) const
{
    auto match = m_variables.find(name);
    if(match == end(m_variables))
    {
        if(m_updatingAni == nullptr)
            throw std::runtime_error(utility::replace(utility::translateKey("GetVariable"), name));
        return m_updatingAni->getValueOf(name);
    }
    return match->second;
}

void AnimationContainer::onSetValueOf(const std::string& name, const float value)
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

void AnimationContainer::updated(const sf::RenderWindow& screen, const float time, const sf::Vector2i& mouseOffset)
{
    updateCurrentTime(time);
    updateLayout(static_cast<sf::Vector2f>(screen.getSize()));
    auto currentPosition = getCurrentPosition();
    for(auto animation = begin(graphics.getAnimations()); animation != end(graphics.getAnimations()); ++animation)
    {
        auto ani = (*animation).get();
        if(ani->isStopped())
            continue;
        m_updatingAni = ani;
        
        ani->setPosition(currentPosition.x, currentPosition.y);
        ani->update();
    }
    m_updatingAni = nullptr;
}

void AnimationContainer::doDraw(const DrawParameter& param)
{
    graphics.draw(param);
}