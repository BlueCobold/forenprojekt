
#include "Entity.hpp"
#include "../animation/CloneHandler.hpp"
#include "../Utility.hpp"

#include <exception>

#include <SFML/Graphics/RenderTarget.hpp>

#include <Box2D/Dynamics/b2World.h>

Entity::Entity(Type type, CloneHandler& cloneHandler, bool respawnable, bool autoKill) :
    m_type(type),
    m_killed(false),
    m_respawnable(respawnable),
    m_autoKill(autoKill),
    m_collideWithBall(true),
    m_collideWithOtherEntity(true),
    m_updatingAni(nullptr),
    m_animationAngle(0),
    m_killAnimationEntity(nullptr),
    m_cloneHandler(cloneHandler)
{
}

Entity::~Entity()
{
}

void Entity::update(const float value)
{
    if(!m_killed)
    {
        updateCurrentTime(value);
        updateKinematics(getPassedTime(), value - m_lastTime);

        auto pos = getPosition();
        auto x = utility::toPixel(pos.x);
        auto y = utility::toPixel(pos.y);
        setValueOf("position.x", x);
        setValueOf("position.y", y);

        bool running = false;
        for(auto animation = begin(getAnimations()); animation != end(getAnimations()); ++animation)
        {
            auto ani = (*animation).get();
            if(ani->isStopped())
                continue;
            m_updatingAni = ani;
            ani->setPosition(x, y);
            if(getBody() != nullptr)
                ani->setRotation(getBody()->GetAngle());
            else if(m_animationAngle != 0)
                ani->setRotation(m_animationAngle);
            ani->update();
            running |= !ani->isStopped();
            m_updatingAni = nullptr;
        }
        updateJoints();

        for(auto sound = begin(m_otherSounds); sound != end(m_otherSounds); ++sound)
            (*sound)->update();

        m_lastTime = value;

        if(!running && m_autoKill)
            kill();
    }

    if(isStopped())
        kill();
}

float Entity::onGetValueOf(const std::string& name) const
{
    auto match = m_variables.find(name);
    if(match == end(m_variables))
    {
        if(m_updatingAni == nullptr)
            throw std::runtime_error(utility::replace(utility::translateKey("GetVariable"), name));
        return m_updatingAni->getValueOf(name);
    }
    return match->second;
}

void Entity::onSetValueOf(const std::string& name, const float value)
{
    if(m_updatingAni == nullptr)
        m_variables[name] = value;
    else
    {
        auto match = m_variables.find(name);
        if(match == end(m_variables))
            m_updatingAni->setValueOf(name, value);
        else
            m_variables[name] = value;
    }
}

void Entity::setName(std::string name)
{
    m_name = name;
}

const Entity::Type& Entity::getType() const
{
    return m_type;
}

void Entity::onRestarted()
{
    m_lastTime = getCurrentTime();
    m_killed = false;

    for(auto animation = begin(getAnimations()); animation != end(getAnimations()); ++animation)
        (*animation)->reset();
}

void Entity::kill()
{
    m_killed = true;
}

bool Entity::killed() const
{
    return m_killed;
}

bool Entity::isRespawnable() const
{
    return m_respawnable;
}

void Entity::setCollideWithBall(bool value)
{
    m_collideWithBall = value;
}

bool Entity::doesCollideWithBall()
{
    return m_collideWithBall;
}

void Entity::setCollideWithOtherEntity(bool value)
{
    m_collideWithOtherEntity = value;
    if(m_collideWithOtherEntity && m_type == SpecialEntity)
        m_type = None;
    else if(!m_collideWithOtherEntity && m_type == None)
        m_type = SpecialEntity;
}

bool Entity::doesCollideWithOtherEntity()
{
    return m_collideWithOtherEntity;
}

void Entity::bindCollisionSound(std::unique_ptr<SoundObject> sound)
{
    m_collisionSound = std::move(sound);
}

void Entity::bindOtherSounds(std::vector<std::unique_ptr<SoundTrigger>> otherSounds)
{
    m_otherSounds = std::move(otherSounds);
}

void Entity::bindCollisionHandler(std::unique_ptr<CollisionHandler> handler)
{
    m_collisionHandler.push_back(std::move(handler));
}

void Entity::bindCollisionFilter(std::unique_ptr<CollisionFilter> filter)
{
    m_collisionFilter = std::move(filter);
}

void Entity::onCollide(Entity& partner, const b2Vec2& point, const float impulse)
{
    if(m_collisionSound != nullptr)
    {
        float volume = std::min(100.f, std::max(impulse/4.f, 0.f));
        if(volume > 10 || m_collisionSound->hasFixedVolume())
            m_collisionSound->play(volume);
    }

    for(auto it = begin(m_collisionHandler); it != end(m_collisionHandler); ++it)
        (*it)->onCollision(*this, partner, point, impulse);
}

bool Entity::shouldCollide(Entity& partner)
{
    if(m_collisionFilter == nullptr)
        return true;
    return m_collisionFilter->shouldCollide(*this, partner);
}

void Entity::applyOverrides(const std::function<void(Animation*)> function)
{
    for(auto it = begin(getAnimations()); it != end(getAnimations()); ++it)
        function(it->get());
}

const Entity* Entity::getKillAnimationEntity() const
{
    return m_killAnimationEntity;
}

void Entity::bindKillAnimationEntity(const Entity* entity)
{
    m_killAnimationEntity = entity;
}

void Entity::setAnimationAngle(const float angle)
{
    m_animationAngle = angle;
}

bool compareDrawOrder(const std::unique_ptr<Entity>& lhs, const std::unique_ptr<Entity>& rhs)
{
    return lhs->getDrawOrder() < rhs->getDrawOrder();
}

std::unique_ptr<Entity> Entity::doClone() const
{
    auto clone = std::unique_ptr<Entity>(new Entity(m_type, m_cloneHandler, m_respawnable, m_autoKill));

    clone->m_name = m_name;
    clone->m_lastTime = m_lastTime;
    clone->m_killed = m_killed;
    clone->m_collideWithBall = m_collideWithBall;
    clone->m_collideWithBall = m_collideWithOtherEntity;
    clone->m_variables = m_variables;
    clone->m_animationAngle = m_animationAngle;
    clone->setDrawOrder(getDrawOrder());

    // copy physical property
    clone->copyFrom(this);

    m_cloneHandler.registerCloneAll(*this, *clone.get());

    for(auto animation = begin(getAnimations()); animation != end(getAnimations()); ++animation)
        clone->bindAnimation((*animation)->clone());

    m_cloneHandler.unregisterCloneAll(*this);

    return clone;
}
