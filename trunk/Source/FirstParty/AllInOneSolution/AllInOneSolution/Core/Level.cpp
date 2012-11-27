#include "Level.hpp"
#include "Config.hpp"
#include "Entity.hpp"
#include "resources/ResourceManager.hpp"
#include "resources/LevelFileLoader.hpp"
#include "Utility.hpp" // toString, toMeter
#include "Teeter.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/System/Err.hpp>

#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Common/b2Draw.h>

#include <tinyxml2.h>

#include <memory> // unique_ptr
#include <ostream> // endl
#include <string>
#include <utility> // pair, make_pair, move

Level::Level(const unsigned int level, ResourceManager& resourceManager, Config& config) :
    m_number(level),
    m_resourceManager(resourceManager),
    m_world(b2Vec2(0.f, 9.81f)),
    m_timeStep(1.f/60.f),
    m_velocityIterations(6),
    m_positionIterations(2),
    m_config(config)
{
    load();
}

Level::~Level()
{

}

void Level::restartAt(const float time)
{
    TimedObject::restartAt(time);
    for(auto it = m_entities.begin(); it != m_entities.end(); ++it)
        (*it)->restartAt(time);
}

void Level::update(const float dt)
{
    m_world.Step(m_timeStep, m_velocityIterations, m_positionIterations);

    for(auto it = m_entities.begin(); it != m_entities.end(); ++it)
        (*it)->update(dt);
}

void Level::draw(sf::RenderWindow& screen)
{
	screen.draw(*m_background);	
	for(auto it = m_entities.begin(); it != m_entities.end(); ++it)
        screen.draw(**it);
    //DebugDraw d(screen);
    //d.SetFlags(b2Draw::e_shapeBit | b2Draw::e_centerOfMassBit);
    //m_world.SetDebugDraw(&d);
    //m_world.DrawDebugData();
}

bool Level::load()
{
    if(m_number == 0) // Level start from 1
        return false;

    tinyxml2::XMLDocument doc;
    doc.LoadFile(filename().c_str());
    
    if(doc.ErrorID() != 0) // Error while loading file
        return false;

	tinyxml2::XMLElement* backgroundXml = doc.FirstChildElement("level")->FirstChildElement("background");
    tinyxml2::XMLElement* objects = doc.FirstChildElement("level")->FirstChildElement("objects");
    tinyxml2::XMLElement* world = doc.FirstChildElement("level")->FirstChildElement("world");
    tinyxml2::XMLElement* grid = doc.FirstChildElement("level")->FirstChildElement("grid");
    
    std::vector<b2BodyDef> bodies;
    tinyxml2::XMLElement* element = nullptr; // Temp object

	//Load background-image
	std::unique_ptr<Background> background(new Background());
	if(backgroundXml != nullptr)
    {
        element = backgroundXml->FirstChildElement("animation");
        if(element != nullptr)
            background->bindAnimation(std::move(LevelFileLoader::parseAnimation(element, background.get(), m_resourceManager)));
    }
	else
		sf::err() << "Bad background key ('" << backgroundXml->Attribute("file") << ")" << std::endl;
	m_background = std::move(background);

    // Iterate over all defined entities
    for(tinyxml2::XMLElement* entityIterator = objects->FirstChildElement("entity");
        entityIterator != nullptr; entityIterator = entityIterator->NextSiblingElement("entity"))
    {
        std::unique_ptr<Entity> entity(new Entity);
        entity->setName(std::string(entityIterator->Attribute("name")));

        // Load animation
        element = entityIterator->FirstChildElement("animation");
        if(element != nullptr)
            entity->bindAnimation(std::move(LevelFileLoader::parseAnimation(element, entity.get(), m_resourceManager)));

        m_entities.push_back(std::move(entity));

        // Load physics
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
        bodyDef.position = b2Vec2(utility::toMeter(element->FloatAttribute("x")), utility::toMeter(element->FloatAttribute("y")));
        bodyDef.angle = element->FloatAttribute("angle");
        bodyDef.fixedRotation = element->BoolAttribute("fixedRotation");
        bodyDef.angularDamping = element->BoolAttribute("angularDamping");

        // Load fixtures
        std::vector<b2FixtureDef> fixtures;

        for(tinyxml2::XMLElement* fixtureIterator = physics->FirstChildElement("fixture");
            fixtureIterator != nullptr; fixtureIterator = fixtureIterator->NextSiblingElement("fixture"))
        {
            // Load shape
            element = fixtureIterator->FirstChildElement("shape");
            if(std::string(element->Attribute("type")) == "polygon") // Load polygon
            {
                std::vector<b2Vec2> vertices;

                // Ierator over the vertices
                for(tinyxml2::XMLElement* vertexIterator = element->FirstChildElement("vertex");
                    vertexIterator != nullptr; vertexIterator = vertexIterator->NextSiblingElement("vertex"))
                {
                    vertices.push_back(b2Vec2(utility::toMeter(vertexIterator->FloatAttribute("x")),
                        utility::toMeter(vertexIterator->FloatAttribute("y"))));
                }

                // Construct the b2Shape
                std::unique_ptr<b2PolygonShape> ps(new b2PolygonShape);
                ps->Set(vertices.data(), vertices.size());
                m_shapes.push_back(std::move(ps));
            }
            else if(std::string(element->Attribute("type")) == "circle") // Load circle
            {
                std::unique_ptr<b2CircleShape> cs(new b2CircleShape);
                cs->m_radius = utility::toMeter(element->FloatAttribute("radius"));
                m_shapes.push_back(std::move(cs));
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

        m_entities.back()->bindBody(body);
    }

    // Iterate over all defined teeter
    for(tinyxml2::XMLElement* teeterIterator = objects->FirstChildElement("teeter");
        teeterIterator != nullptr; teeterIterator = teeterIterator->NextSiblingElement("teeter"))
    {
        b2PolygonShape ps;

        // Load physics
        tinyxml2::XMLElement* physics = teeterIterator->FirstChildElement("physics");

        // Load body
        element = physics->FirstChildElement("body");

        b2Vec2 position = b2Vec2(utility::toMeter(element->FloatAttribute("x")), utility::toMeter(element->FloatAttribute("y")));
        float angle = element->FloatAttribute("angle");
        b2Vec2 center = b2Vec2(utility::toMeter(element->FloatAttribute("centerX")), utility::toMeter(element->FloatAttribute("centerY")));

        // Load fixture
        b2FixtureDef fixtureDef;

        for(tinyxml2::XMLElement* fixtureIterator = physics->FirstChildElement("fixture");
            fixtureIterator != nullptr; fixtureIterator = fixtureIterator->NextSiblingElement("fixture"))
        {
            // Load shape
            element = fixtureIterator->FirstChildElement("shape");
            if(std::string(element->Attribute("type")) == "polygon") // Load polygon
            {
                std::vector<b2Vec2> vertices;

                // Ierator over the vertices
                for(tinyxml2::XMLElement* vertexIterator = element->FirstChildElement("vertex");
                    vertexIterator != nullptr; vertexIterator = vertexIterator->NextSiblingElement("vertex"))
                {
                    vertices.push_back(b2Vec2(utility::toMeter(vertexIterator->FloatAttribute("x")),
                        utility::toMeter(vertexIterator->FloatAttribute("y"))));
                }

                // Construct the b2Shape
                ps.Set(vertices.data(), vertices.size());

                fixtureDef.shape = &ps;
                fixtureDef.density = fixtureIterator->FloatAttribute("density");
                fixtureDef.friction = fixtureIterator->FloatAttribute("friction");
                fixtureDef.restitution = fixtureIterator->FloatAttribute("restitution");


            }
        }

        std::unique_ptr<Teeter> teeter(new Teeter(position.x, position.y, center.x, center.y, fixtureDef, m_world, m_config.get<float>("MouseScale")));

        // Teeter is animated
        element = teeterIterator->FirstChildElement("animation");
        if(element != nullptr)
            teeter->bindAnimation(std::move(LevelFileLoader::parseAnimation(element, teeter.get(), m_resourceManager)));

        m_entities.push_back(std::move(teeter));
    }

    // Load world properties
    tinyxml2::XMLElement* gravity = world->FirstChildElement("gravity");
    m_world.SetGravity(b2Vec2(gravity->FloatAttribute("x"), gravity->FloatAttribute("y")));

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
