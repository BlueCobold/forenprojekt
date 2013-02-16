#pragma once

#ifndef CHANGE_PROPERTY_COLLISION_HANDLER_HPP
#define CHANGE_PROPERTY_COLLISION_HANDLER_HPP

#include "CollisionHandler.hpp"
#include "../../animation/OrientedObject.hpp"
#include "../../animation/provider/ValueProvider.hpp"
#include "../../animation/TimedObject.hpp"
#include "../../animation/VariableHandler.hpp"

#include <memory>
#include <string>

class ChangePropertyCollisionHandler :
    public CollisionHandler,
    public AnimatedObject
{
public:
    ChangePropertyCollisionHandler(const std::string& name) 
        : m_name(name),
        m_entityA(nullptr),
        m_entityB(nullptr)
    { }

    void bindProvider(std::unique_ptr<ValueProvider> provider)
    {
        m_provider = std::move(provider);
    }

    virtual void OnCollision(Entity* entityA, Entity* entityB)
    {
        m_entityA = entityA;
        m_entityB = entityB;
        if(m_name.length()>=5 && m_name.substr(0, 5)=="ball:")
            entityB->setValueOf(m_name.substr(5), m_provider->getValue());
        else
            entityA->setValueOf(m_name, m_provider->getValue());
        m_entityA = nullptr;
        m_entityB = nullptr;
    }
    
    virtual float getValueOf(const std::string& name) const
    {
        if (name.length()>=5 && name.substr(0, 5)=="ball:")
        {
            if (m_entityB == nullptr)
                throw std::exception("Can't get a ball-variable at this time.");
            return m_entityB->getValueOf(name.substr(5));
        }
        if (m_entityA == nullptr)
            throw std::exception("Can't get an entity-variable at this time.");
        return m_entityA->getValueOf(name);
    }

    virtual void setValueOf(const std::string& name, const float value)
    {
        if (name.length()>=5 && name.substr(5)=="ball:")
        {
            if (m_entityB == nullptr)
                throw std::exception("Can't set an entity-variable at this time.");
        }
        if (m_entityA == nullptr)
            throw std::exception("Can't set an entity-variable at this time.");
        return m_entityA->setValueOf(name, value);
    }

    float getPassedTime() const
    {
        if (m_entityA == nullptr)
            throw std::exception("Can't receive a timestamp at this time.");
        return m_entityA->getPassedTime();
    }

    virtual float getAngle() const
    {
        if (m_entityA == nullptr)
            throw std::exception("Can't receive a timestamp at this time.");
        return m_entityA->getAngle();
    }

private:
    std::unique_ptr<ValueProvider> m_provider;
    std::string m_name;
    Entity* m_entityA;
    Entity* m_entityB;
};

#endif // CHANGE_PROPERTY_COLLISION_HANDLER_HPP
