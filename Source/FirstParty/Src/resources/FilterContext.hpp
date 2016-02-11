#pragma once

#ifndef FILTER_CONTEXT_HPP
#define FILTER_CONTEXT_HPP

#include <functional>
#include <stdexcept>

class CloneHandler;
class Entity;
class VariableHandler;

struct b2Vec2;

struct FilterContext
{
    typedef std::function<void(const b2Vec2& position, const Entity& spawn)> SpawnFunc;

    b2Vec2& gravity;
    SpawnFunc spawnFunc;
    VariableHandler& variableHandler;
    CloneHandler& cloneHandler;

    FilterContext(b2Vec2& gravity, SpawnFunc spawnFunc, VariableHandler& variableHandler, CloneHandler& cloneHandler) :
        gravity(gravity),
        spawnFunc(spawnFunc),
        variableHandler(variableHandler),
        cloneHandler(cloneHandler)
    { }
};

#endif // FILTER_CONTEXT_HPP
