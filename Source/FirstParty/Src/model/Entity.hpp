#pragma once

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "AnimatedGraphics.hpp"
#include "PhysicalObject.hpp"
#include "SoundTrigger.hpp"
#include "collision/handler/CollisionHandler.hpp"
#include "collision/filter/CollisionFilter.hpp"

#include <functional>
#include <memory> // std::unique_ptr
#include <string>

/// This class will be used to draw objects that have a binding
/// to Box2D 
class Entity : public PhysicalObject, public AnimatedGraphics
{
public:

    enum Type {Teeter, Ball, Target, BonusTarget, None};

private:

    std::string m_name;
    Type m_type;
    float m_lastTime;
    bool m_killed;
    bool m_collideWithBall;
    Animation* m_updatingAni;
    std::vector<std::unique_ptr<CollisionHandler>> m_collisionHandler;
    std::unique_ptr<CollisionFilter> m_collisionFilter;
    std::map<std::string, float> m_variables;
    bool m_respawnable;
    bool m_autoKill;
    float m_animationAngle;
    std::unique_ptr<SoundObject> m_collisionSound;
    std::vector<std::unique_ptr<SoundTrigger>> m_otherSounds;
    const Entity* m_killAnimationEntity;

public:

    Entity(Type type, bool respawnable = false, bool autoKill = false);
    virtual ~Entity();

    virtual void update(const float value);
    virtual void restartAt(const float value) override;

    void setName(std::string name);
    const Type& getType() const;

    void kill();
    bool killed() const;
    bool isRespawnable() const;

    void setCollideWithBall(bool value);
    bool doesCollideWithBall();
    void bindCollisionSound(std::unique_ptr<SoundObject> sound);
    void bindOtherSounds(std::vector<std::unique_ptr<SoundTrigger>> otherSounds);

    virtual float getValueOf(const std::string& name) const override;
    virtual void setValueOf(const std::string& name, const float value) override;

    void bindCollisionHandler(std::unique_ptr<CollisionHandler> handler);
    void bindCollisionFilter(std::unique_ptr<CollisionFilter> filter);
    void onCollide(Entity* partner, const b2Vec2& point, const float impulse);
    bool shouldCollide(Entity* partner);

    void applyOverrides(const std::function<void(Animation*)> function);
    void setAnimationAngle(const float angle);

    void bindKillAnimationEntity(const Entity* entity);
    const Entity* getKillAnimationEntity() const;
};

bool compareDrawOrder(const std::unique_ptr<Entity>& lhs, const std::unique_ptr<Entity>& rhs);

#endif // ENTITY_HPP
