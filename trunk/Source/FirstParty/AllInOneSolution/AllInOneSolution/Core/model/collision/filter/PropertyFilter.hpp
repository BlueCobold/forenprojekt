#pragma once

#ifndef PROPERTY_FILTER
#define PROPERTY_FILTER

#include "CollisionFilter.hpp"
#include "../../../animation/OrientedObject.hpp"
#include "../../../animation/provider/ValueProvider.hpp"
#include "../../../animation/TimedObject.hpp"
#include "../../../animation/VariableHandler.hpp"

#include <memory>
#include <string>

class PropertyFilter :
    public CollisionFilter,
    public AnimatedObject
{
public:
    PropertyFilter(const bool useEntityProperties)
        : m_entityA(nullptr),
        m_entityB(nullptr),
        m_useValuesFromA(useEntityProperties)
    { }

    void bindProvider(std::unique_ptr<ValueProvider> provider)
    {
        m_provider = std::move(provider);
    }

    virtual bool ShouldCollide(Entity* entityA, Entity* entityB)
    {
        if(m_provider == nullptr)
            throw std::exception("Provider in property filter is null.");
        m_entityA = entityA;
        m_entityB = entityB;
        float val = m_provider->getValue();
        m_entityA = nullptr;
        m_entityB = nullptr;
        return val >= 1;
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
                throw std::exception("Can't set a ball-variable at this time.");
            m_entityB->setValueOf(name, value);
            return;
        }
        if (m_entityA == nullptr)
            throw std::exception("Can't set an entity-variable at this time.");
        return m_entityA->setValueOf(name, value);
    }

    virtual float getPassedTime() const
    {
        if (m_useValuesFromA)
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
        if (m_useValuesFromA)
        {
            if (m_entityA == nullptr)
                throw std::exception("Can't receive an entity-angle at this time.");
            return m_entityA->getAngle();
        }
        else
        {
            if (m_entityB == nullptr)
                throw std::exception("Can't receive a ball-angle at this time.");
            return m_entityB->getAngle();
        }
    }

private:
    std::unique_ptr<ValueProvider> m_provider;
    bool m_useValuesFromA;
    Entity* m_entityA;
    Entity* m_entityB;
};

#endif // PROPERTY_FILTER
