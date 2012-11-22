#include "Level.hpp"

#include "ResourceManager.hpp"
#include "Utility.hpp" // toString
#include "Entity.hpp"

#include <SFML/System/Err.hpp>

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Common/b2Math.h>

#include <tinyxml2.h>

#include <utility> // pair, make_pair, move
#include <memory> // unique_ptr
#include <string>
#include <iostream> // endl

Level::Level(const unsigned int level, ResourceManager& resourceManager) :
    m_number(level),
    m_resourceManager(resourceManager),
    m_world(b2Vec2(0.f, 9.81f))
{
    load();
}

Level::~Level()
{

}

bool Level::load()
{
    if(m_number == 0) // Level start from 1
        return false;

    tinyxml2::XMLDocument doc;
    doc.LoadFile(filename().c_str());
    
    if(doc.ErrorID() != 0) // Error while loading file
        return false;

    tinyxml2::XMLElement* objects = doc.FirstChildElement("level")->FirstChildElement("objects");
    tinyxml2::XMLElement* grid = doc.FirstChildElement("level")->FirstChildElement("grid");
    
    std::vector<b2BodyDef> bodies;
    tinyxml2::XMLElement* element = nullptr; // Temp object

    // Iterate over all defined entities
    for(tinyxml2::XMLElement* entityIterator = objects->FirstChildElement("entity");
        entityIterator != nullptr; entityIterator = entityIterator->NextSiblingElement("entity"))
    {
        Entity entity;

        // Entity is animated
        if(entityIterator->FirstChildElement("animation") != nullptr)
        {
            // Load animation
            element = entityIterator->FirstChildElement("animation");
            if(element != nullptr)
                entity.bindAnimation(element->BoolAttribute("infinite"), element->FloatAttribute("min"),
                    element->FloatAttribute("step"), element->IntAttribute("num"), element->IntAttribute("width"),
                    element->IntAttribute("height"));
        }

        m_entities.push_back(std::move(entity));

        // Load texture
        element = entityIterator->FirstChildElement("texture");
        if(m_resourceManager.getTexture(element->Attribute("file")) != nullptr)
            m_entities.back().bindTexture(*m_resourceManager.getTexture(element->Attribute("file")),
                sf::IntRect(0, 0, element->IntAttribute("width"), element->IntAttribute("height")));
        else
            sf::err() << "Bad texture key for entity: " << entityIterator->Attribute("name") << std::endl;

        // Load physics
        std::vector<b2FixtureDef> fixtures;

        tinyxml2::XMLElement* physics = entityIterator->FirstChildElement("physics");

        // Load body
        element = physics->FirstChildElement("body");
        b2BodyDef bodyDef;
        if(std::string(element->Attribute("type")) == "static")
            bodyDef.type = b2_staticBody;
        else if(std::string(element->Attribute("type")) == "kinematics")
            bodyDef.type = b2_kinematicBody;
        else if(std::string(element->Attribute("type")) == "dynamic")
            bodyDef.type = b2_dynamicBody;
        bodyDef.position = b2Vec2(element->FloatAttribute("x"), element->FloatAttribute("y"));
        bodyDef.angle = element->FloatAttribute("angle");
        bodyDef.fixedRotation = element->BoolAttribute("fixedRotation");

        // Load fixtures
        for(tinyxml2::XMLElement* fixtureIterator = physics->FirstChildElement("fixture");
            fixtureIterator != nullptr; fixtureIterator = fixtureIterator->NextSiblingElement("fixture"))
        {
            // Load shape
            element = fixtureIterator->FirstChildElement("shape");
            if(std::string(element->Attribute("type")) == "polygon") // Load polygon
            {
                std::vector<b2Vec2> vertices;

                for(tinyxml2::XMLElement* vertexIterator = element->FirstChildElement("vertex");
                    vertexIterator != nullptr; vertexIterator = vertexIterator->NextSiblingElement("vertex"))
                {
                    vertices.push_back(b2Vec2(vertexIterator->FloatAttribute("x"), vertexIterator->FloatAttribute("y")));
                }

                std::unique_ptr<b2PolygonShape> ps(new b2PolygonShape);
                ps->Set(vertices.data(), vertices.size());
                m_shapes.push_back(std::move(ps));
            }

            b2FixtureDef fixtureDef;
            fixtureDef.shape = m_shapes.back().get();
            fixtureDef.density = fixtureIterator->FloatAttribute("density");
            fixtureDef.friction = fixtureIterator->FloatAttribute("friction");
            fixtureDef.restitution = fixtureIterator->FloatAttribute("restitution");

            fixtures.push_back(fixtureDef);
        }

        // Create & bind body
        b2Body* body = m_world.CreateBody(&bodyDef);
        for(auto it = fixtures.begin(); it != fixtures.end(); it++)
            body->CreateFixture(&(*it));

        m_entities.back().bindBody(body);
    }

    return true;
}

const unsigned int Level::number()
{
    return m_number;
}

const std::string Level::filename()
{
    std::string filename("res/level/");
    if(m_number < 10)
        filename.append("00" + utility::toString(m_number) + ".lvl");
    else if(m_number < 100)
        filename.append("0" + utility::toString(m_number) + ".lvl");
    else
        filename.append(utility::toString(m_number) + ".lvl");

    return filename;
}
