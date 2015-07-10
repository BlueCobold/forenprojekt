#pragma once

#ifndef ENTITY_FACTORY_HPP
#define ENTITY_FACTORY_HPP

#include "Entity.hpp"
#include "../animation/provider/RandomProvider.hpp"
#include <iostream>

class EntityFactory : public Entity
{
private:
    std::unique_ptr<Entity> m_product;
    float m_manufactureMoment;
    b2Vec2 m_spawnOffset;
    std::string m_productName;
    RandomProvider m_randomGenerator;

    bool m_ready;
public:
    EntityFactory(CloneHandler& cloneHandler,
                  bool respawnable, 
                  bool autoKill,
                  std::unique_ptr<Entity>& product,
                  float minDelayTime,
                  float maxDelayTime,
                  const b2Vec2& spawnOffset);

    virtual void update(const float value);

    void deliver(std::vector<std::unique_ptr<Entity>>& packet);
};

#endif