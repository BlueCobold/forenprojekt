#pragma once

#ifndef PHYSICS_PARSER_HPP
#define PHYSICS_PARSER_HPP

#include "ProviderParser.hpp"
#include "../model/Array.hpp"

#include <memory>
#include <vector>

#include "Box2D/Box2D.h"
#include "tinyxml2.h"

class PhysicsParser
{
public:
    struct Physics
    {
        struct Controllers
        {
            Array<std::unique_ptr<ValueProvider>, 2> position;
            std::unique_ptr<ValueProvider> rotation;
            
            Controllers()
            { }

            Controllers(Controllers&& other)
            {
                position = std::move(other.position);
                rotation = std::move(other.rotation);
            }

            Controllers& operator= (Controllers&& other)
            {
                position = std::move(other.position);
                rotation = std::move(other.rotation);
                return *this;
            }
        };

        b2BodyDef bodyDef;
        b2FixtureDef fixtureDef;
        Controllers controllers;
        std::vector<std::unique_ptr<b2Shape>> shapes;
        
        Physics()
        { }

        Physics(Physics&& other)
        {
            controllers = std::move(other.controllers);
            bodyDef = other.bodyDef;
            fixtureDef = other.fixtureDef;
            shapes = std::move(other.shapes);
        }

        Physics& operator= (Physics&& other)
        {
            controllers = std::move(other.controllers);
            bodyDef = other.bodyDef;
            fixtureDef = other.fixtureDef;
            shapes = std::move(other.shapes);
            return *this;
        }
    };

    PhysicsParser(ProviderContext context) :
        m_context(context),
        m_providerParser(context)
    { }

    Physics parse(const tinyxml2::XMLElement& physic,
                  const tinyxml2::XMLElement& shape) const;

private:
    Physics parseBody(const tinyxml2::XMLElement& xml) const;

    Physics::Controllers parseKinematics(const tinyxml2::XMLElement& xml) const;

    ProviderContext m_context;
    ProviderParser m_providerParser;
};

#endif // PHYSICS_PARSER_HPP
