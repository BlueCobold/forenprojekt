#include "StateManager.hpp"

#include <utility>
#include <exception>
#include <sstream>

StateManager::StateManager() :
    m_currentTime(m_frametime.getElapsedTime().asSeconds()),
    m_currentState(nullptr),
    m_currentStateId(None),
    m_paused(false)
{
}

void StateManager::registerState(StateId id, std::unique_ptr<State> state)
{
    auto existingState = m_statesById.find(id);
    if(existingState != end(m_statesById))
        throw std::runtime_error(utility::replace(utility::translateKey("@StateExist"), utility::toString(id)));

    m_statesById[id] = std::move(state);
}

void StateManager::setState(StateId id, EnterStateInformation* enterInformation)
{
    if(m_currentState != nullptr)
        m_currentState->pause(m_currentTime);

    auto state = getState(id);
    if(id == TransitionStateId)
    {
        EnterTransitionStateInformation* info = dynamic_cast<EnterTransitionStateInformation*>(enterInformation);
        info->m_source = m_currentState;
        info->m_target = getState(info->m_followingState);
    }
    m_currentStateId = id;
    m_currentState = state;

    EnterStateInformation info;
    if(enterInformation == nullptr)
        enterInformation = &info;
    enterInformation->m_prepareOnly = false;
    m_currentState->onEnter(enterInformation, m_currentTime);
    if(m_paused)
        m_currentState->pause(m_currentTime);
    else
        m_currentState->resume(m_currentTime);
}

State* StateManager::getState(StateId id) const
{
    auto state = m_statesById.find(id);
    if(state == end(m_statesById))
        throw std::runtime_error(utility::replace(utility::translateKey("@StateUnknow"), utility::toString(id)));

    return state->second.get();
}

void StateManager::update()
{
    m_currentTime = m_frametime.getElapsedTime().asMicroseconds() / 1000000.0;
    if(m_currentState == nullptr)
        return;

    StateChangeInformation changeInformation = StateChangeInformation::Empty();
    do
    {
        changeInformation = m_currentState->update(m_currentTime);
        if(changeInformation != StateChangeInformation::Empty())
            setState(changeInformation.getStateId(), changeInformation.getUserData());
    } while(changeInformation != StateChangeInformation::Empty());
}

void StateManager::passEvent(utility::Event::EventType type)
{
    if(m_currentState != nullptr && type != utility::Event::NoEvent)
    {
        if(type == utility::Event::LostFocus)
            m_paused = true;
        else if(type == utility::Event::GainFocus)
            m_paused = false;

        m_currentState->onEvent(type, m_frametime.getElapsedTime().asSeconds());
    }
}

void StateManager::draw(const DrawParameter& parameter)
{
    if(m_currentState != nullptr)
    {
        m_currentState->draw(parameter);
    }
}
