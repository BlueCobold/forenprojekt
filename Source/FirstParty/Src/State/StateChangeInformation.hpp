#pragma once

#ifndef STATECHANGEINFORMATION_HPP
#define STATECHANGEINFORMATION_HPP

#include "State.hpp"
#include "EnterStateInformation.hpp"

class StateChangeInformation
{
public:

    static StateChangeInformation Empty() { return StateChangeInformation(None, nullptr); }

public:

    StateChangeInformation(StateId id, EnterStateInformation* userData):
        m_stateId(id),
        m_userData(userData) { }

    ~StateChangeInformation() {}

    bool operator==(const StateChangeInformation& information)
    {
        return this->m_stateId == information.m_stateId && this->m_userData == information.m_userData;
    }

    bool operator!=(const StateChangeInformation& information)
    {
        return this->m_stateId != information.m_stateId || this->m_userData != information.m_userData;
    }

    StateId getStateId() const { return m_stateId; }
    EnterStateInformation* getUserData() const { return m_userData; }

private:

    StateId m_stateId;
    EnterStateInformation* m_userData;
};

#endif