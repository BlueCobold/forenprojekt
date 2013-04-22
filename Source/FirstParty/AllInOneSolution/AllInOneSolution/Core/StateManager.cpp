#include "StateManager.hpp"

#include "LoadLevelState.hpp"

#include <utility> // move
#include <exception>
#include <sstream>

StateManager::StateManager(sf::RenderWindow& screen) :
    m_screen(screen),
	m_currentState(nullptr),
    m_currentStateId(None),
    m_currentLevel(nullptr)
{

}

void StateManager::registerState(StateId id, std::shared_ptr<State> state)
{
	auto existingState = m_statesById.find(id);
	if(existingState != end(m_statesById))
	{
		std::stringstream ss;
		ss << "The state with id " << id << " already exists.";
		throw std::runtime_error(ss.str());
	}

	m_statesById[id] = state;		
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
    if(m_currentStateId == LoadLevelStateId)
        m_currentLevel = std::move((dynamic_cast<LoadLevelState*>(m_currentState))->gainLevel());
    if(m_currentLevel != nullptr)
        enterInformation->m_level = m_currentLevel.get();
    m_currentStateId = id;
	m_currentState = state;
    m_currentState->onEnter(enterInformation, m_currentTime);
}

State* StateManager::getState(StateId id) const
{
    auto state = m_statesById.find(id);
	if(state == end(m_statesById))
	{
		std::stringstream ss;
		ss << "The state with id " << id << " does not exist.";
		throw std::runtime_error(ss.str());
	}
    return state->second.get();
}

void StateManager::update()
{
    m_currentTime = m_frametime.getElapsedTime().asSeconds();
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

void StateManager::draw()
{
    if(m_currentState !=nullptr )
	{
		m_currentState->draw(m_screen);
	}
}
