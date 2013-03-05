#pragma once

#ifndef CHANGE_PROPERTY_COLLISION_HANDLER_HPP
#define CHANGE_PROPERTY_COLLISION_HANDLER_HPP

#include "CollisionHandler.hpp"
#include "../../../animation/OrientedObject.hpp"
#include "../../../animation/provider/ValueProvider.hpp"
#include "../../../animation/TimedObject.hpp"
#include "../../../animation/VariableHandler.hpp"

#include <memory>
#include <string>

class ChangePropertyCollisionHandler :
    public CollisionHandler,
    public AnimatedObject
{
public:
    ChangePropertyCollisionHandler(const std::string& name, VariableHandler* handler = nullptr) 
        : m_name(name),
        m_entityA(nullptr),
        m_entityB(nullptr),
        m_useValuesFromA((name.length() < 5) || (name.substr(0, 5) != "ball:")),
        m_useGlobal(handler != nullptr && (name.length() > 7) && (name.substr(0, 7) == "global:")),
        m_globalHandler(handler)
    {
        if(m_useGlobal)
            m_trimmedVar = m_name.substr(7);
        else
        {
            if(m_useValuesFromA)
                m_trimmedVar = m_name;
            else
                m_trimmedVar = m_name.substr(5);
        }
    }

    void bindProvider(std::unique_ptr<ValueProvider> provider)
    {
        m_provider = std::move(provider);
    }

    virtual void onCollision(Entity* entityA, Entity* entityB)
    {
        if(m_provider == nullptr)
            throw std::exception("Provider in property handler is null.");
        m_entityA = entityA;
        m_entityB = entityB;
        if(m_useGlobal)
            m_globalHandler->setValueOf(m_trimmedVar, m_provider->getValue());
        else
        {
            if(m_useValuesFromA)
                entityA->setValueOf(m_trimmedVar, m_provider->getValue());
            else
                entityB->setValueOf(m_trimmedVar, m_provider->getValue());
        }
        m_entityA = nullptr;
        m_entityB = nullptr;
    }
    
    virtual float getValueOf(const std::string& name) const
    {
        if(m_globalHandler != nullptr && name.length() > 7 && name.substr(0,7) == "global:")
            return m_globalHandler->getValueOf(name.substr(7));

        if(name.length() >= 5 && name.substr(0, 5) == "ball:")
        {
            if(m_entityB == nullptr)
                throw std::exception("Can't get a ball-variable at this time.");
            return m_entityB->getValueOf(name.substr(5));
        }
        if(m_entityA == nullptr)
            throw std::exception("Can't get an entity-variable at this time.");
        return m_entityA->getValueOf(name);
    }

    virtual void setValueOf(const std::string& name, const float value)
    {
        if(m_globalHandler != nullptr && name.length() > 7 && name.substr(0,7) == "global:")
            return m_globalHandler->setValueOf(name.substr(7), value);

        if(name.length() >= 5 && name.substr(5) == "ball:")
        {
            if(m_entityB == nullptr)
                throw std::exception("Can't set a ball-variable at this time.");
            m_entityB->setValueOf(name.substr(5), value);
            return;
        }
        if(m_entityA == nullptr)
            throw std::exception("Can't set an entity-variable at this time.");
        return m_entityA->setValueOf(name, value);
    }

    virtual float getPassedTime() const
    {
        if(m_useValuesFromA)
        {
            if(m_entityA == nullptr)
                throw std::exception("Can't receive an entity-timestamp at this time.");
            return m_entityA->getPassedTime();
        }
        else
        {
            if(m_entityB == nullptr)
                throw std::exception("Can't receive a ball-timestamp at this time.");
            return m_entityB->getPassedTime();
        }
    }

    virtual float getAngle() const
    {
        if(m_useValuesFromA)
        {
            if (m_entityA == nullptr)
                throw std::exception("Can't receive an entity-angle at this time.");
            return m_entityA->getAngle();
        }
        else
        {
            if(m_entityB == nullptr)
                throw std::exception("Can't receive a ball-angle at this time.");
            return m_entityB->getAngle();
        }
    }

private:
    std::unique_ptr<ValueProvider> m_provider;
    VariableHandler* m_globalHandler;
    bool m_useValuesFromA;
    bool m_useGlobal;
    std::string m_name;
    std::string m_trimmedVar;
    Entity* m_entityA;
    Entity* m_entityB;
};

#endif // CHANGE_PROPERTY_COLLISION_HANDLER_HPP
