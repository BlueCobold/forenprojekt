#pragma once

#ifndef ENTERSTATEINFORMATION_HPP
#define ENTERSTATEINFORMATION_HPP

//#include "../model/Level.hpp"
#include "State.hpp"

#include <memory>

class Level;

class State;

class EnterStateInformation
{
public:
    virtual ~EnterStateInformation()
    { }

    Level* m_level;
    bool m_prepareOnly;
    int m_levelNumber;
};

class EnterPauseStateInformation : public EnterStateInformation
{
public:
    float m_levelTime;
};

class EnterTransitionStateInformation : public EnterStateInformation
{
public:
    StateId m_followingState;
    StateId m_comeFromeState;
    EnterStateInformation* m_onEnterInformation;

    State* m_source;
    State* m_target;

    int m_transitionType;
};

class EnterPlayStateInformation : public EnterStateInformation
{
public:
    bool m_returnFromPause;
};

class EnterOptionStateInformation : public EnterStateInformation
{
public:
    StateId m_comeFromState;
};

class EnterHighScoreStateInformation : public EnterStateInformation
{
public:
    StateId m_comeFromState;
};
#endif // ENTERSTATEINFORMATION_HPP