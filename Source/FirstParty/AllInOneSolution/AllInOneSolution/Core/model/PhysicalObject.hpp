#pragma once

#ifndef PHYSICAL_OBJECT_HPP
#define PHYSICAL_OBJECT_HPP

#include "../animation/OrientedObject.hpp"
#include "../animation/provider/ValueProvider.hpp"

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Collision/Shapes/b2Shape.h>
#include <Box2D/Dynamics/b2Body.h>

#include <memory> // unique_ptr
#include <vector>

/// This class will be used to give a object a binding to
/// a physical body
class PhysicalObject : public virtual OrientedObject
{
private:

    std::unique_ptr<ValueProvider> m_rotation;
    std::unique_ptr<ValueProvider> m_xPositionProvider;
    std::unique_ptr<ValueProvider> m_yPositionProvider;
    b2Vec2 m_basePosition;
    
    b2Body* m_body;
    b2FixtureDef m_fixtureDef;
    b2BodyDef m_bodyDef;
    b2World* m_world;

    bool m_basePosChanged;

public:

    PhysicalObject() : m_world(nullptr), m_body(nullptr), m_basePosChanged(false)
    { }

    virtual ~PhysicalObject()
    { }

    void bindDefs(b2FixtureDef fixtureDef, b2BodyDef bodyDef, b2World* world)
    {
        m_fixtureDef = fixtureDef;
        m_bodyDef = bodyDef;
        m_world = world;
    }

    /// Bind the body
    void bindBody()
    {
        if(m_world == nullptr || m_body != nullptr)
            return;
        // Create & bind body
        m_body = m_world->CreateBody(&m_bodyDef);
        m_body->CreateFixture(&m_fixtureDef);
        // Save the entity in the body for the contact listener
        m_body->SetUserData(this);
        if(m_basePosChanged)
            m_body->SetTransform(m_basePosition, m_bodyDef.angle);
        else
            m_basePosition = m_body->GetPosition();
    }

    void unbindBody()
    {
        m_world->DestroyBody(m_body);
    }

    b2Body* getBody()
    {
        return m_body;
    }

    void bindRotationController(std::unique_ptr<ValueProvider> provider)
    {
        m_rotation = std::move(provider);
    }

    void bindPositionController(std::unique_ptr<ValueProvider> x, std::unique_ptr<ValueProvider> y)
    {
        m_xPositionProvider = std::move(x);
        m_yPositionProvider = std::move(y);
    }

    virtual float getAngle() const
    {
        return m_body->GetAngle();
    }

    void updateKinematics(const float value, const float delta);

	const b2Vec2& getPosition() const
	{
		return m_body->GetPosition();
	}

    void setPosition(const b2Vec2& pos)
    {
        if(m_body != nullptr)
            m_body->SetTransform(pos, 0.0f);
        else
        {
            m_basePosition = pos;
            m_basePosChanged = true;
        }
    }

protected:

    const b2Vec2& getStartPosition() const
    {
        return m_basePosition;
    }
};

#endif // PHYSICAL_OBJECT_HPP
