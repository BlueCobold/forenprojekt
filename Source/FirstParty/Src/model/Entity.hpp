#pragma once

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "AnimatedGraphics.hpp"
#include "PhysicalObject.hpp"
#include "SoundTrigger.hpp"
#include "../animation/Cloneable.hpp"
#include "collision/handler/CollisionHandler.hpp"
#include "collision/filter/CollisionFilter.hpp"
class CloneHandler;

#include <functional>
#include <memory> // std::unique_ptr
#include <string>

/// This class will be used to draw objects that have a binding
/// to Box2D
class Entity : public PhysicalObject, public AnimatedGraphics, public Cloneable<Entity>
{
public:

    enum Type
    {
        Teeter,
        Ball,
        Target,
        BonusTarget,
        EntityFactory,
        SpecialEntity,
        None
    };

private:

    std::string m_name;
    Type m_type;
    double m_lastTime;
    bool m_killed;
    bool m_respawnable;
    bool m_autoKill;
    bool m_collideWithBall;
    bool m_collideWithOtherEntity;
    Animation* m_updatingAni;
    std::vector<std::unique_ptr<CollisionHandler>> m_collisionHandler;
    std::unique_ptr<CollisionFilter> m_collisionFilter;
    std::map<std::string, double> m_variables;
    float m_animationAngle;
    std::unique_ptr<SoundObject> m_collisionSound;
    std::vector<std::unique_ptr<SoundTrigger>> m_otherSounds;
    const Entity* m_killAnimationEntity;
    CloneHandler& m_cloneHandler;
    bool m_propagatePosition;

    std::unique_ptr<Entity> doClone() const override;
    virtual void timeUpdated();
    virtual void updated();

protected:
    void onRestarted() override;
    double onGetValueOf(const std::string& name) const override;
    void onSetValueOf(const std::string& name, const double value) override;

public:

    Entity(Type type, CloneHandler& cloneHandler, bool respawnable = false, bool autoKill = false);
    virtual ~Entity();

    void update(const double time);

    void setName(std::string name);
    const Type& getType() const;

    void kill();
    bool killed() const;
    bool isRespawnable() const;
    void propagatePosition(bool value)
    {
        m_propagatePosition = value;
    }

    void setCollideWithBall(bool value);
    bool doesCollideWithBall();
    void setCollideWithOtherEntity(bool value);
    bool doesCollideWithOtherEntity();
    void bindCollisionSound(std::unique_ptr<SoundObject> sound);
    void bindOtherSounds(std::vector<std::unique_ptr<SoundTrigger>> otherSounds);

    void bindCollisionHandler(std::unique_ptr<CollisionHandler> handler);
    void bindCollisionFilter(std::unique_ptr<CollisionFilter> filter);
    void onCollide(Entity& partner, const b2Vec2& point, const float impulse);
    bool shouldCollide(Entity& partner);

    void applyOverrides(const std::function<void(Animation*)> function);
    void setAnimationAngle(const float angle);

    void bindKillAnimationEntity(const Entity* entity);
    const Entity* getKillAnimationEntity() const;
};

bool compareDrawOrder(const std::unique_ptr<Entity>& lhs, const std::unique_ptr<Entity>& rhs);

#endif // ENTITY_HPP
