#include "EntityFactory.hpp"

EntityFactory::EntityFactory(CloneHandler& cloneHandler,
                             bool respawnable, 
                             bool autoKill,
                             const std::string& productName,
                             float minDelayTime,
                             float maxDelayTime,
                             const b2Vec2& spawnOffset) :
    Entity(Entity::EntityFactory, cloneHandler, respawnable, autoKill),
    m_randomGenerator(minDelayTime, maxDelayTime),
    m_productName(productName),
    m_spawnOffset(spawnOffset),
    m_manufactureMoment(0),
    m_ready(false),
    m_product(nullptr)
{ }

void EntityFactory::update(const float value)
{
    Entity::update(value);

    if(m_manufactureMoment == 0 || value > m_manufactureMoment)
    {
        if(value > m_manufactureMoment)
            m_ready = true;

        m_manufactureMoment = value + m_randomGenerator.getValue();
    }
}

void EntityFactory::deliver(std::vector<std::unique_ptr<Entity>>& packet)
{
    if(m_ready && m_product)
    {
        m_ready = false;
        m_product->setPosition(m_spawnOffset + getPosition());
        packet.push_back(m_product->clone());
    }
}

void EntityFactory::setProduct(Entity* product)
{
    if(product)
        m_product = product->clone();
    else
        m_product = nullptr;
}

std::string EntityFactory::getProductName() const
{
    return m_productName;
}