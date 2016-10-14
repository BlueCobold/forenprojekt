#include "EntityFactory.hpp"

EntityFactory::EntityFactory(CloneHandler& cloneHandler,
                             bool respawnable,
                             bool autoKill,
                             std::unique_ptr<Entity> product,
                             float minDelayTime,
                             float maxDelayTime,
                             const b2Vec2& spawnOffset) :
    Entity(Entity::EntityFactory, cloneHandler, respawnable, autoKill),
    m_product(std::move(product)),
    m_manufactureMoment(0),
    m_spawnOffset(spawnOffset),
    m_randomGenerator(minDelayTime, maxDelayTime),
    m_transferOwnSpeed(false)
{
    if(minDelayTime <= 0 || maxDelayTime <= 0)
        throw std::runtime_error(utility::translateKey("InvalidFactoryTime"));
}

void EntityFactory::update(const float value)
{
    Entity::update(value);

    while(m_manufactureMoment < getPassedTime() && m_callback != nullptr)
    {
        auto product = m_product->clone();
        product->setPosition(m_spawnOffset + getPosition());
        if(m_transferOwnSpeed)
            product->setSpawnSpeed(getBody()->GetLinearVelocity());

        m_callback(product);
        m_manufactureMoment += m_randomGenerator.getValue();
    }
}

void EntityFactory::onRestarted()
{
    Entity::onRestarted();
    m_manufactureMoment = 0;
}

void EntityFactory::registerForDelivery(DeliveryCallback callback)
{
    m_callback = callback;
}

void EntityFactory::setOwnSpeedTransfer(bool value)
{
    m_transferOwnSpeed = value;
}