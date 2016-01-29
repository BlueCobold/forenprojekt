#pragma once

#ifndef PROVIDER_PARSER_CONTEXT_HPP
#define PROVIDER_PARSER_CONTEXT_HPP

#include <unordered_map>

#include "tinyxml2.h"

class VariableHandler;
class Stoppable;
class TimedObject;
class OrientedObject;
class CloneHandler;

class ProviderParserContext
{
public:
    VariableHandler* variableHandler;
    Stoppable* stoppable;
    TimedObject* timeSource;
    OrientedObject* angleSource;
    CloneHandler& cloneHandler;
    std::unordered_map<std::string, const tinyxml2::XMLElement*> functions;
    std::unordered_map<std::string, Stoppable*> customStopTags;

    ProviderParserContext(VariableHandler* variableHandler,
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

    ProviderParserContext& withFunctions(std::unordered_map<std::string, const tinyxml2::XMLElement*> functions)
    {
        this->functions = functions;
        return *this;
    }

    ProviderParserContext& withCustomTag(const std::string& name, Stoppable* target) {
        customStopTags[name] = target;
        return *this;
    }

    ProviderParserContext& withStopTarget(Stoppable* target) {
        stoppable = target;
        return *this;
    }
};

#endif // PROVIDER_PARSER_CONTEXT_HPP
