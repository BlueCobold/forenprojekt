
#include "CloneHandler.hpp"

TimeProvider::CloneCallback CloneHandler::createTimeProviderCloneHandler()
{
    return [this](TimeProvider::CallbackParam& original) -> const TimedObject& { return replace(original, timeReplacements); };
}

AngleProvider::CloneCallback CloneHandler::createAngleProviderCloneHandler()
{
    return [this](AngleProvider::CallbackParam& original) -> const OrientedObject& { return replace(original, oriReplacements); };
}

Stop::CloneCallback CloneHandler::createStopProviderCloneHandler()
{
    return [this](Stop::CallbackParam& original) -> Stoppable& { return replace(original, stopReplacements); };
}

VariableProvider::CloneCallback CloneHandler::createVariableProviderCloneHandler()
{
    return [this](VariableProvider::CallbackParam& original) -> const VariableHandler& { return replace(original, varReplacements); };
}

SetVariable::CloneCallback CloneHandler::createSetVariableProviderCloneHandler()
{
    return [this](SetVariable::CallbackParam& original) -> VariableHandler& { return replace(original, setReplacements); };
}
