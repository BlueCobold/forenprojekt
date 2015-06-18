#pragma once

#ifndef CLONE_HANDLER_HPP
#define CLONE_HANDLER_HPP

#include "../animation/provider/AngleProvider.hpp"
#include "../animation/provider/TimeProvider.hpp"
#include "../animation/provider/VariableProvider.hpp"
#include "../animation/provider/SetVariable.hpp"
#include "../animation/provider/Stop.hpp"
#include "../animation/Animation.hpp"

class CloneHandler : public Animation::CloneHandler
{
public:
    virtual AngleProvider::CloneCallback createAngleProviderCloneHandler() = 0;
    virtual TimeProvider::CloneCallback createTimeProviderCloneHandler() = 0;
    virtual Stop::CloneCallback createStopProviderCloneHandler() = 0;
    virtual VariableProvider::CloneCallback createVariableProviderCloneHandler() = 0;
    virtual SetVariable::CloneCallback createSetVariableProviderCloneHandler() = 0;
};

#endif // CLONE_HANDLER_HPP