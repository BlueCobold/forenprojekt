#pragma once

#ifndef COLLIDER_CONTEXT_HPP
#define COLLIDER_CONTEXT_HPP

#include <SFML/System/Vector2.hpp>

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>

class CloneHandler;
class Entity;
class Ball;
class VariableHandler;
class TimedObject;

struct b2Vec2;

struct ColliderContext
{
    typedef std::function<void(const b2Vec2& position, const Entity& spawn)> SpawnFunc;
    typedef std::function<void(int time)> DeltaTimeFunc;
    typedef std::function<Ball&()> BallGetter;
    typedef std::function<void(const sf::Vector2f& position, const std::string& fontName, const std::string& text)> SpawnLabelFunc;
    typedef std::function<bool()> IsInvulFunc;

    SpawnFunc spawnFunc;
    DeltaTimeFunc timeFunc;
    BallGetter ball;
    SpawnLabelFunc labelFunc;
    IsInvulFunc isInvul;
    VariableHandler& variableHandler;
    TimedObject& timeSource;
    CloneHandler& cloneHandler;

    ColliderContext(SpawnFunc spawnFunc,
                    DeltaTimeFunc timeFunc,
                    BallGetter ball,
                    SpawnLabelFunc labelFunc,
                    IsInvulFunc isInvul,
                    VariableHandler& variableHandler,
                    TimedObject& timeSource,
                    CloneHandler& cloneHandler) :
        spawnFunc(spawnFunc),
        timeFunc(timeFunc),
        ball(ball),
        labelFunc(labelFunc),
        isInvul(isInvul),
        variableHandler(variableHandler),
        timeSource(timeSource),
        cloneHandler(cloneHandler)
    {
        if(spawnFunc == nullptr)
            throw std::runtime_error("empty spawnFunc in filter");

        if(timeFunc == nullptr)
            throw std::runtime_error("empty timeFunc in filter");

        if(labelFunc == nullptr)
            throw std::runtime_error("empty labelFunc in filter");

        if(ball == nullptr)
            throw std::runtime_error("empty ball in filter");

        if(isInvul == nullptr)
            throw std::runtime_error("empty isInvul in filter");
    }
};

#endif // COLLIDER_CONTEXT_HPP
