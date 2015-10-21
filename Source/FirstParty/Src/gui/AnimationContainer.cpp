
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

std::unique_ptr<MenuElement> AnimationContainer::clone() const
{
    auto other = std::unique_ptr<AnimationContainer>(new AnimationContainer(getPosition(), getOffset(), getId(), m_cloneHandler));
    m_cloneHandler.registerCloneAll(*this, *other.get());
    
    other->setVisibleWhenId(getVisibleWhenId());

    for(auto it = begin(m_variables); it != end(m_variables); ++it)
        other->m_variables[it->first] = it->second;

    for(auto it = begin(getAnimations()); it != end(getAnimations()); ++it)
        other->bindAnimation((*it)->clone());

    m_cloneHandler.unregisterCloneAll(*this);

    return std::move(other);
}

float AnimationContainer::getValueOf(const std::string& name) const
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

void AnimationContainer::setValueOf(const std::string& name, const float value)
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

void AnimationContainer::update(const sf::RenderWindow& screen, const float time, const sf::Vector2i& mouseOffset)
{
    updateCurrentTime(time);
    for(auto animation = begin(getAnimations()); animation != end(getAnimations()); ++animation)
    {
        auto ani = (*animation).get();
        if(ani->isStopped())
            continue;
        m_updatingAni = ani;
        ani->setPosition(getPosition().x + getOffset().x, getPosition().y + getOffset().x);
        ani->update();
    }
    m_updatingAni = nullptr;
}

void AnimationContainer::draw(const DrawParameter& param)
{
    GraphicalObject::draw(param);
}

void AnimationContainer::updateLayout(const sf::Vector2f& screenSize)
{
    MenuElement::updateLayout(screenSize);
}