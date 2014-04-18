#include "AnimationContainer.hpp"

AnimationContainer::AnimationContainer(const sf::Vector2f& position, int id) :
    MenuElement(id, MenuElementType::Animation, position, sf::Vector2f(0, 0)),
    m_updatingAni(nullptr)
{
}

AnimationContainer::AnimationContainer(AnimationContainer&& toMove) :
    MenuElement(toMove.getId(), MenuElementType::Animation, toMove.getPosition(), toMove.getOffset()),
    m_updatingAni(toMove.m_updatingAni)
{
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
        ani->setPosition(getPosition().x, getPosition().y);
        ani->update();
    }
    m_updatingAni = nullptr;
}

void AnimationContainer::draw(const DrawParameter& param)
{
    GraphicalObject::draw(param);
}
