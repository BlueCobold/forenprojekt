#pragma once

#ifndef PROVIDER_CONTEXT_HPP
#define PROVIDER_CONTEXT_HPP

#include <tinyxml2.h>

#include <string>
#include <unordered_map>

class VariableHandler;
class Stoppable;
class TimedObject;
class OrientedObject;
class CloneHandler;

class ProviderContext
{
public:
    VariableHandler* variableHandler;
    Stoppable* stoppable;
    TimedObject* timeSource;
    OrientedObject* angleSource;
    CloneHandler& cloneHandler;
    std::unordered_map<std::string, const tinyxml2::XMLElement*> functions;
    std::unordered_map<std::string, Stoppable*> customStopTags;

    ProviderContext(VariableHandler* variableHandler,
                    Stoppable* stopTarget,
                    TimedObject* timeSource,
                    OrientedObject* angleSource,
                    CloneHandler& cloneHandler) :
        variableHandler(variableHandler),
        stoppable(stopTarget),
        timeSource(timeSource),
        angleSource(angleSource),
        cloneHandler(cloneHandler)
    { }

    ProviderContext& withFunctions(const std::unordered_map<std::string, const tinyxml2::XMLElement*>& functions)
    {
        this->functions = functions;
        return *this;
    }

    ProviderContext& withCustomTag(const std::string& name, Stoppable* target) {
        customStopTags[name] = target;
        return *this;
    }

    ProviderContext& withStopTarget(Stoppable* target) {
        stoppable = target;
        return *this;
    }
};

#endif // PROVIDER_CONTEXT_HPP
