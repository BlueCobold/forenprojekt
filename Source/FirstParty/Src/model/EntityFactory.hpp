#pragma once

#ifndef ENTITY_FACTORY_HPP
#define ENTITY_FACTORY_HPP

#include "Entity.hpp"
#include "../animation/provider/RandomProvider.hpp"

#include <functional>
#include <memory>

class EntityFactory : public Entity
{
public:
    typedef std::function<void(std::unique_ptr<Entity>&)> DeliveryCallback;

    EntityFactory(CloneHandler& cloneHandler,
                  bool respawnable, 
                  bool autoKill,
                  std::unique_ptr<Entity> product,
                  float minDelayTime,
                  float maxDelayTime,
                  const b2Vec2& spawnOffset);

    void registerForDelivery(DeliveryCallback callback);
    void setOwnSpeedTransfer(bool value);

private:
    std::unique_ptr<Entity> m_product;
    double m_manufactureMoment;
    b2Vec2 m_spawnOffset;
    RandomProvider m_randomGenerator;
    DeliveryCallback m_callback;
    bool m_transferOwnSpeed;

    void onRestarted() override;
    void updated() override;
};

#endif