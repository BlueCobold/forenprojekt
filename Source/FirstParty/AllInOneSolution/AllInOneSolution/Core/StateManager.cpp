#include "StateManager.hpp"

#include <utility> // move

StateManager::StateManager() :
    m_index(0)
{

}

StateManager::~StateManager()
{

}

void StateManager::push(std::unique_ptr<State> state)
{
    m_states.push_back(std::move(state));
}

void StateManager::next()
{
    if((m_states[m_index] != m_states.back() && !m_states.empty()))
    {
        m_index++;
    }
}

void StateManager::prev()
{
    if(( m_states[m_index] != m_states.front() && !m_states.empty()))
    {
        m_index--;
    }
}

void StateManager::update()
{
    if(!m_states.empty())
        if(!m_states[m_index]->isPaused())
            m_states[m_index]->update();
}

void StateManager::draw()
{
    if(!m_states.empty())
        if(!m_states[m_index]->isPaused())
            m_states[m_index]->draw();
}