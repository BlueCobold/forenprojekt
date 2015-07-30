#pragma once

#ifndef ENTERSTATEINFORMATION_HPP
#define ENTERSTATEINFORMATION_HPP


#include <memory>
#ifdef LEVELTESTING
#include <string>
#endif

class Level;

class State;

class EnterStateInformation
{
public:
    EnterStateInformation() :
        m_level(nullptr),
        m_prepareOnly(false),
        m_levelNumber(1)
    { }

    virtual ~EnterStateInformation()
    { }

    Level* m_level;
    bool m_prepareOnly;
    int m_levelNumber;
};

class EnterPauseStateInformation : public EnterStateInformation
{
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
    EnterOptionStateInformation(StateId fromStateId) :
        m_comeFromState(fromStateId)
    { }

    StateId m_comeFromState;
};

class EnterHighScoreStateInformation : public EnterStateInformation
{
public:
    StateId m_comeFromState;
};

class EnterCoinShopStateInformation : public EnterStateInformation
{
public:
    StateId m_comeFromState;
};

class EnterLoadLevelStateInformation : public EnterStateInformation
{
public:
    EnterLoadLevelStateInformation() : 
        m_directPlay(false)
    { }

    bool m_directPlay;
#ifdef LEVELTESTING
    std::string m_file;
#endif
};

#endif // ENTERSTATEINFORMATION_HPP
