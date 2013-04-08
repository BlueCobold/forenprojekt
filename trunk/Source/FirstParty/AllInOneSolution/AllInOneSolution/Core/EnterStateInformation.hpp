#pragma once

#ifndef ENTERSTATEINFORMATION_HPP
#define ENTERSTATEINFORMATION_HPP

#include "model/Level.hpp"

#include <memory>

struct EnterPauseStateInformation
{
    std::unique_ptr<sf::Texture> m_background;
    Level* m_level;
    bool m_enterPauseTransition;
};

struct EnterTransitionStateInformation
{
    Transition* m_transition;
    StateId m_followingState;
    void* m_onEnterInformation;
};

struct EnterPlayStateInformation
{
    Level* m_level;
    bool m_returnFromPause;
};


#endif // ENTERSTATEINFORMATION_HPP