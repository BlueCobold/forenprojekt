#pragma once

#ifndef CLONE_HANDLER_PROVIDER_HPP
#define CLONE_HANDLER_PROVIDER_HPP

#include "CloneHandler.hpp"
#include "../animation/AnimatedObject.hpp"

#include <unordered_map>

class CloneHandlerProvider : public CloneHandler
{
public:
    
    virtual void onCloneBegin(const Animation& source, Animation& target)
    {
        stopReplacements.insert(std::make_pair(&source, &target));
        varReplacements.insert(std::make_pair(&source, &target));
        setReplacements.insert(std::make_pair(&source, &target));
    }

    virtual void onCloneEnd(const Animation& source, Animation& target)
    {
        stopReplacements.erase(stopReplacements.find(&source));
        varReplacements.erase(varReplacements.find(&source));
        setReplacements.erase(setReplacements.find(&source));
    }
        
    template<typename Source>
    static std::unique_ptr<Source> clone(const Cloneable<Source>& source,
        const TimedObject& timeTarget,
        const OrientedObject& orientationTarget,
        Stoppable& stopTarget,
        const VariableHandler& readVariableTarget,
        VariableHandler& writeVariableTarget)
    {
        timeReplacements.insert(std::make_pair(&source, &timeTarget));
        oriReplacements.insert(std::make_pair(&source, &orientationTarget));
        stopReplacements.insert(std::make_pair(&source, &stopTarget));
        varReplacements.insert(std::make_pair(&source, &readVariableTarget));
        setReplacements.insert(std::make_pair(&source, &writeVariableTarget));
        auto cloned = source.clone();
        timeReplacements.erase(timeReplacements.find(&source));
        oriReplacements.erase(oriReplacements.find(&source));
        stopReplacements.erase(stopReplacements.find(&source));
        varReplacements.erase(varReplacements.find(&source));
        setReplacements.erase(setReplacements.find(&source));
        return std::move(cloned);
    }

    virtual TimeProvider::CloneCallback createTimeProviderCloneHandler() override
    {
        return [=](TimeProvider::CallbackParam* original){ return CloneHandlerProvider::replace(original, timeReplacements); };
    }

    virtual AngleProvider::CloneCallback createAngleProviderCloneHandler() override
    {
        return [=](AngleProvider::CallbackParam* original){ return CloneHandlerProvider::replace(original, oriReplacements); };
    }

    virtual Stop::CloneCallback createStopProviderCloneHandler() override
    {
        return [=](Stop::CallbackParam* original){ return CloneHandlerProvider::replace(original, stopReplacements); };
    }

    virtual VariableProvider::CloneCallback createVariableProviderCloneHandler() override
    {
        return [=](VariableProvider::CallbackParam* original){ return CloneHandlerProvider::replace(original, varReplacements); };
    }

    virtual SetVariable::CloneCallback createSetVariableProviderCloneHandler() override
    {
        return [=](SetVariable::CallbackParam* original){ return CloneHandlerProvider::replace(original, setReplacements); };
    }
    
private:
    static std::unordered_map<const void*, const TimedObject*> timeReplacements;
    static std::unordered_map<const void*, const OrientedObject*> oriReplacements;
    static std::unordered_map<const void*, Stoppable*> stopReplacements;
    static std::unordered_map<const void*, const VariableHandler*> varReplacements;
    static std::unordered_map<const void*, VariableHandler*> setReplacements;

    template<typename Provider, typename Observed>
    static Observed* replace(Provider* original, std::unordered_map<const void*, Observed*>& map)
    {
        auto replacement = map.find(original->getObserved());
        if(replacement != end(map))
            return replacement->second;
        return original->getObserved();
    }
};

#endif // CLONE_HANDLER_PROVIDER_HPP