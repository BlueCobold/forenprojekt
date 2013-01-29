#include "StateManager.hpp"

#include <utility> // move
#include <exception>
#include <sstream>

StateManager::StateManager() :
	m_currentState(nullptr)
{

}

void StateManager::registerState(StateId id, std::shared_ptr<State> state)
{
	auto existingState = m_statesById.find(id);
	if(existingState != m_statesById.end())
	{
		std::stringstream ss;
		ss << "The state with id " << id << " already exists.";
		throw std::exception(ss.str().c_str());
	}

	m_statesById[id] = state;		
}

void StateManager::setState(StateId id, void* enterInformation)
{
	auto state = m_statesById.find(id);
	if(state == m_statesById.end())
	{
		std::stringstream ss;
		ss << "The state with id " << id << " does not exist.";
		throw std::exception(ss.str().c_str());
	}
	m_currentState = state->second;
	m_currentState->onEnter(enterInformation);
}

void StateManager::update()
{
	if(m_currentState.get() && !m_currentState->isPaused())
	{
		StateChangeInformation changeInformation = m_currentState->update();
		if(changeInformation != StateChangeInformation::Empty())
			setState(changeInformation.getStateId(), changeInformation.getUserData());
	}
}

void StateManager::draw()
{
    if(m_currentState.get() && !m_currentState->isPaused())
	{
		m_currentState->draw();
	}
}
