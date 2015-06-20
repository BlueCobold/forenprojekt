#pragma once

#ifndef CLONE_HANDLER_PROVIDER_HPP
#define CLONE_HANDLER_PROVIDER_HPP

#include "CloneHandler.hpp"
#include "Animation.hpp"

#include "provider/AngleProvider.hpp"
#include "provider/TimeProvider.hpp"
#include "provider/VariableProvider.hpp"
#include "provider/SetVariable.hpp"
#include "provider/Stop.hpp"

#include <unordered_map>

class CloneHandler
{
public:
    
    template<typename T>
    const T* addr(const T& source)
    {
        const T& t = source;
        return &t;
    }

    template<typename Source>
    void registerClone(const Source& source,
        Stoppable& stopTarget,
        const VariableHandler& readVariableTarget,
        VariableHandler& writeVariableTarget)
    {
        stopReplacements.insert(std::make_pair(addr<Stoppable>(source), &stopTarget));
        varReplacements.insert(std::make_pair(addr<VariableHandler>(source), &readVariableTarget));
        setReplacements.insert(std::make_pair(addr<VariableHandler>(source), &writeVariableTarget));
    }

    template<typename Source>
    void registerCloneAll(const Source& source, Source& target)
    {
        timeReplacements.insert(std::make_pair(addr<TimedObject>(source), &target));
        oriReplacements.insert(std::make_pair(addr<OrientedObject>(source), &target));
        stopReplacements.insert(std::make_pair(addr<Stoppable>(source), &target));
        varReplacements.insert(std::make_pair(addr<VariableHandler>(source), &target));
        setReplacements.insert(std::make_pair(addr<VariableHandler>(source), &target));
    }
    
    template<typename Source>
    void unregisterClone(const Source& source)
    {
        stopReplacements.erase(stopReplacements.find(addr<Stoppable>(source)));
        varReplacements.erase(varReplacements.find(addr<VariableHandler>(source)));
        setReplacements.erase(setReplacements.find(addr<VariableHandler>(source)));
    }

    template<typename Source>
    void unregisterCloneAll(const Source& source)
    {
        timeReplacements.erase(timeReplacements.find(addr<TimedObject>(source)));
        oriReplacements.erase(oriReplacements.find(addr<OrientedObject>(source)));
        stopReplacements.erase(stopReplacements.find(addr<Stoppable>(source)));
        varReplacements.erase(varReplacements.find(addr<VariableHandler>(source)));
        setReplacements.erase(setReplacements.find(addr<VariableHandler>(source)));
    }

    TimeProvider::CloneCallback createTimeProviderCloneHandler()
    {
        return [this](TimeProvider::CallbackParam* original){ return replace(original, timeReplacements); };
    }

    AngleProvider::CloneCallback createAngleProviderCloneHandler()
    {
        return [this](AngleProvider::CallbackParam* original){ return replace(original, oriReplacements); };
    }

    Stop::CloneCallback createStopProviderCloneHandler()
    {
        return [this](Stop::CallbackParam* original){ return replace(original, stopReplacements); };
    }

    VariableProvider::CloneCallback createVariableProviderCloneHandler()
    {
        return [this](VariableProvider::CallbackParam* original){ return replace(original, varReplacements); };
    }

    SetVariable::CloneCallback createSetVariableProviderCloneHandler()
    {
        return [this](SetVariable::CallbackParam* original){ return replace(original, setReplacements); };
    }

private:
    std::unordered_map<const void*, const TimedObject*> timeReplacements;
    std::unordered_map<const void*, const OrientedObject*> oriReplacements;
    std::unordered_map<const void*, Stoppable*> stopReplacements;
    std::unordered_map<const void*, const VariableHandler*> varReplacements;
    std::unordered_map<const void*, VariableHandler*> setReplacements;

    template<typename Provider, typename Observed>
    Observed* replace(Provider* original, std::unordered_map<const void*, Observed*>& map)
    {
        auto replacement = map.find(original->getObserved());
        if(replacement != end(map))
            return replacement->second;
        return original->getObserved();
    }
};

#endif // CLONE_HANDLER_PROVIDER_HPP
