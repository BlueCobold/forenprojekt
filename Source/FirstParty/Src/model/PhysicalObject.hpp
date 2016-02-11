#pragma once

#ifndef PHYSICAL_OBJECT_HPP
#define PHYSICAL_OBJECT_HPP

#include "../animation/OrientedObject.hpp"
#include "../animation/provider/ValueProvider.hpp"

#include "joint/SingleDistanceJoint.hpp"
#include "joint/SinglePrismaticJoint.hpp"
#include "joint/SingleRevoluteJoint.hpp"

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Common/b2BlockAllocator.h>

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
    b2Vec2 m_spawnSpeed;
    bool m_basePosChanged;

    std::vector<b2FixtureDef> m_fixtureDefs;
    b2World* m_world;
    b2Body* m_body;
    b2BodyDef m_bodyDef;
    std::vector<std::unique_ptr<b2Shape>> m_shapes;

    std::vector<std::unique_ptr<JointObject>> m_joints;

public:

    PhysicalObject() :
        m_basePosChanged(false),
        m_world(nullptr),
        m_body(nullptr),
        m_basePosition(b2Vec2(0, 0)),
        m_spawnSpeed(b2Vec2(0, 0))
    { }

    virtual ~PhysicalObject()
    { }

    void bindDefs(
        const b2FixtureDef& fixtureDef,
        std::vector<std::unique_ptr<b2Shape>>& shapes,
        const b2BodyDef& bodyDef, b2World* world)
    {
        bool wasBound = m_world != nullptr && m_body != nullptr;
        if(wasBound)
            unbindBody();
        m_fixtureDefs.clear();
        m_shapes = std::move(shapes);
        m_basePosition = bodyDef.position;
        for(auto shape = begin(m_shapes); shape != end(m_shapes); ++shape)
        {
            b2FixtureDef def;
            def.density = fixtureDef.density;
            def.friction = fixtureDef.friction;
            def.restitution = fixtureDef.restitution;
            def.shape = shape->get();
            m_fixtureDefs.push_back(def);
        }
        m_bodyDef = bodyDef;
        m_world = world;
        if(wasBound)
            bindBody();
    }

    bool hasPhysics()
    {
        return m_world != nullptr;
    }

    /// Bind the body
    void bindBody()
    {
        if(m_world == nullptr || m_body != nullptr)
            return;
        // Create & bind body
        m_body = m_world->CreateBody(&m_bodyDef);
        for(auto fixture = begin(m_fixtureDefs); fixture != end(m_fixtureDefs); ++fixture)
            m_body->CreateFixture(&(*fixture));
        // Save the entity in the body for the contact listener
        m_body->SetUserData(this);
        if(m_basePosChanged)
            m_body->SetTransform(m_basePosition, m_bodyDef.angle);
        else
            m_basePosition = m_body->GetPosition();

        m_body->SetLinearVelocity(m_spawnSpeed);
    }

    void unbindBody()
    {
        if(m_world != nullptr && m_body != nullptr)
        {
            m_world->DestroyBody(m_body);
            m_body = nullptr;
        }
    }

    b2Body* getBody() const
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

    virtual float getAngle() const override
    {
        if(m_body != nullptr)
            return m_body->GetAngle();
        return 0.f;
    }

    void updateKinematics(const float value, const float delta);

    const b2Vec2& getPosition() const
    {
        if(m_body != nullptr)
            return m_body->GetPosition();
        return m_basePosition;
    }

    void setPosition(const b2Vec2& pos)
    {
        if(m_body != nullptr)
        {
            m_body->SetTransform(pos, 0.0f);
            m_basePosition = pos;
        }
        else
        {
            m_basePosition = pos;
            m_basePosChanged = true;
        }
    }

    void freeze()
    {
        if(m_body != nullptr)
            m_body->SetActive(false);
    }

    void unfreeze()
    {
        if(m_body != nullptr)
            m_body->SetActive(true);
    }

    bool frozen() const
    {
        if(m_body != nullptr)
            return !m_body->IsActive();
        return false;
    }

    void addJoint(std::unique_ptr<JointObject> joint)
    {
        m_joints.push_back(std::move(joint));
    }

    bool hasJoints() const
    {
        return m_joints.size() > 0;
    }

    void reinstallJoints()
    {
        if(m_body != nullptr)
            for(auto it = begin(m_joints); it != end(m_joints); ++it)
                (*it)->reinstall(*m_body);
    }

    void setSpawnSpeed(const b2Vec2& speed)
    {
        m_spawnSpeed = speed;
    }
protected:

    const b2Vec2& getStartPosition() const
    {
        return m_basePosition;
    }

    void updateJoints()
    {
        for(auto it = begin(m_joints); it != end(m_joints); ++it)
            (*it)->update();
    }

    void copyFrom(const PhysicalObject* other)
    {
        m_body = nullptr;
        m_fixtureDefs.clear();

        if(other->m_rotation)
            m_rotation = other->m_rotation->clone();
        if(other->m_xPositionProvider)
            m_xPositionProvider = other->m_xPositionProvider->clone();
        if(other->m_yPositionProvider)
            m_yPositionProvider = other->m_yPositionProvider->clone();

        m_basePosition = other->m_basePosition;
        m_basePosChanged = other->m_basePosChanged;
        m_bodyDef = other->m_bodyDef;
        m_world = other->m_world;

        for(auto shape = begin(other->m_shapes); shape != end(other->m_shapes); ++shape)
        {
            if((*shape)->GetType() == b2Shape::e_circle)
            {
                b2CircleShape* newShape = new b2CircleShape;
                *newShape = *(static_cast<b2CircleShape*>(shape->get()));
                m_shapes.push_back(std::unique_ptr<b2Shape>(newShape));
            }
            else if((*shape)->GetType() == b2Shape::e_polygon)
            {
                b2PolygonShape* newShape = new b2PolygonShape;
                *newShape = *(static_cast<b2PolygonShape*>(shape->get()));
                m_shapes.push_back(std::unique_ptr<b2Shape>(newShape));
            }
        }

        for(auto shape = begin(m_shapes); shape != end(m_shapes); ++shape)
        {
            b2FixtureDef def;
            def.density = other->m_fixtureDefs[0].density;
            def.friction = other->m_fixtureDefs[0].friction;
            def.restitution = other->m_fixtureDefs[0].restitution;
            def.shape = shape->get();
            m_fixtureDefs.push_back(def);
        }

        if(hasJoints())
        {
            for(auto joint = begin(other->m_joints); joint != end(other->m_joints); ++joint)
            {
                JointObject* newJoint = nullptr;
                if((*joint)->getType() == JointObject::SingleDistance)
                    newJoint = new SingleDistanceJoint;
                else if((*joint)->getType() == JointObject::SinglePrismatic)
                    newJoint = new SinglePrismaticJoint;
                else if((*joint)->getType() == JointObject::SingleRevolute)
                    newJoint = new SingleRevoluteJoint;
                else
                    continue;

                newJoint->copyFrom(*joint->get());
                if(m_body)
                    newJoint->reinstall(*m_body);

                m_joints.push_back(std::unique_ptr<JointObject>(newJoint));
            }
        }
    }
};

#endif // PHYSICAL_OBJECT_HPP
