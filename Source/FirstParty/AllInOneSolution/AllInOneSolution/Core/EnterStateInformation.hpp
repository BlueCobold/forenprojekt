#pragma once

#ifndef ENTERSTATEINFORMATION_HPP
#define ENTERSTATEINFORMATION_HPP

#include "model/Level.hpp"
#include "rendering\transitions\Transition.hpp"

#include <memory>

struct EnterPauseStateInformation
{
    std::unique_ptr<sf::Texture> m_background;
    std::unique_ptr<Level> m_level;
    bool m_enterPauseTransition;
};

struct EnterTransitionStateInformation
{
    std::unique_ptr<Transition> m_transition;
    StateId m_followingState;
    void* m_onEnterInformation;
};

struct EnterPlayStateInformation
{
    std::unique_ptr<Level> m_level;
    bool m_returnFromPause;
};


#endif // ENTERSTATEINFORMATION_HPP