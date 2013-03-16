#include "Level.hpp"
#include "Teeter.hpp"

#include "../resources/Config.hpp"
#include "../resources/LevelFileLoader.hpp"
#include "../Utility.hpp"

#include "collision/handler/ChangePropertyCollisionHandler.hpp"
#include "collision/filter/Always.hpp"
#include "collision/filter/ChangeGravityFilter.hpp"
#include "collision/filter/Never.hpp"
#include "collision/filter/PropertyFilter.hpp"
#include "collision/filter/SpawnEntity.hpp"

#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>

#include <tinyxml2.h>

bool Level::load()
{
    if(m_number == 0) // Level start from 1
        return false;

    tinyxml2::XMLDocument doc;
    doc.LoadFile(filename().c_str());

    if(!validate(doc)) // Validate the XML file
        throw std::runtime_error(utility::replace(utility::translateKey("InvalidXml"), filename()));

    // Parse templates
    Templates templates;
    if(tinyxml2::XMLElement* xmlTemplates = doc.FirstChildElement("level")->FirstChildElement("templates"))
    {
        if(tinyxml2::XMLElement* shapes = xmlTemplates->FirstChildElement("shapes"))
            templates.shapes = std::move(LevelFileLoader::parseList(shapes, "shape", "name"));

        if(tinyxml2::XMLElement* physics = xmlTemplates->FirstChildElement("physics"))
            templates.physics = std::move(LevelFileLoader::parseList(physics, "physic", "name"));

        if(tinyxml2::XMLElement* functions = xmlTemplates->FirstChildElement("functions"))
            templates.functions = std::move(LevelFileLoader::parseList(functions, "function", "name"));

        if(tinyxml2::XMLElement* entities = xmlTemplates->FirstChildElement("entities"))
        {
            // Add use keys 'name' (objects) and 'rep' (grid)
            templates.entities = std::move(LevelFileLoader::parseList(entities, "entity", "rep"));
            auto temp = LevelFileLoader::parseList(entities, "entity", "name");
            templates.entities.insert(begin(temp), end(temp));
        }
    }
    
    auto constants = doc.FirstChildElement("level")->FirstChildElement("constants");
    if(constants != nullptr)
        LevelFileLoader::parseConstants(constants, this);

    // ==Parse grid==
    tinyxml2::XMLElement* grid = doc.FirstChildElement("level")->FirstChildElement("grid");

    // Get tile size
    unsigned int size = static_cast<unsigned int>(grid->IntAttribute("size"));
    m_height = grid->FloatAttribute("height");
    m_width = grid->FloatAttribute("width");

    // Separate the lines for easier processing
    std::vector<std::string> lines = LevelFileLoader::parseGrid(grid);

    for(unsigned int row = 0; row < lines.size(); ++row)
        for(std::size_t column = 0; column < lines[row].length(); column += 2)
        {
            std::string name = lines[row].substr(column, 2);

            // Ignore empty 'tiles'
            if(name == "  ")
                continue;
            
            std::unique_ptr<Entity> entity = parseEntityFromTemplate(name, templates, sf::Vector2u((column/2)*size, row*size));
            if(entity != nullptr)
                m_entities.push_back(std::move(entity));
        }

    tinyxml2::XMLElement* objects = doc.FirstChildElement("level")->FirstChildElement("objects");
    tinyxml2::XMLElement* backgroundXml = objects->FirstChildElement("background");
    tinyxml2::XMLElement* world = doc.FirstChildElement("level")->FirstChildElement("world");
    
    tinyxml2::XMLElement* element = nullptr; // Temp object

    // Load background-image
    std::unique_ptr<Background> background(new Background(sf::Vector2u(
        static_cast<unsigned int>(m_width),
        static_cast<unsigned int>(m_height))));
    if(backgroundXml != nullptr)
    {
        for(auto parallax = backgroundXml->FirstChildElement("parallax"); parallax != nullptr;
            parallax = parallax->NextSiblingElement("parallax"))
        {
            std::unique_ptr<ParallaxLayer> layer(new ParallaxLayer(sf::Vector2f(
                parallax->FloatAttribute("width"),
                parallax->FloatAttribute("height"))));
            for(auto anim = parallax->FirstChildElement("animation"); anim != nullptr;
                anim = anim->NextSiblingElement("animation"))
                layer->bindAnimation(std::move(LevelFileLoader::parseAnimation(anim, layer.get(), layer.get(), m_resourceManager, &templates.functions)));
            background->bindLayer(std::move(layer));
        }
    }
    m_background = std::move(background);

    for(tinyxml2::XMLElement* entitiesIterator = objects->FirstChildElement("entity");
        entitiesIterator != nullptr; entitiesIterator = entitiesIterator->NextSiblingElement("entity"))
    {
        m_entities.push_back(std::move(parseEntity(entitiesIterator, sf::Vector2u(0, 0), templates)));
    }

    // Load world properties
    tinyxml2::XMLElement* gravity = world->FirstChildElement("gravity");
    m_defaultGravity = b2Vec2(gravity->FloatAttribute("x"), gravity->FloatAttribute("y"));
    m_world.SetGravity(m_defaultGravity);
    m_world.SetContactListener(m_contactListener.get());

    // setup scrollview
    m_scrollView.setLevelSize(sf::Vector2f(getWidth(), getHeight()));

    // get the fucking ball
    for(auto it = begin(m_entities); it != end(m_entities); ++it)
        if((*it)->getType() == Entity::Ball)
        {
            m_ball = dynamic_cast<Ball*>((*it).get());
            m_ball->setFieldDimension(b2Vec2(m_width,m_height));
        }

    m_remainingTarget = m_totalTarget;

    return true;
}

std::unique_ptr<Entity> Level::parseEntityFromTemplate(
    std::string name,
    Templates& templates,
    const sf::Vector2u& position,
    bool bindInstantly)
{
    auto match = templates.entities.find(name);
    if(match == end(templates.entities))
        return nullptr;

    return parseEntity(match->second, position, templates, bindInstantly);
}

std::unique_ptr<Entity> Level::parseEntity(
    tinyxml2::XMLElement* entity,
    const sf::Vector2u& position,
    Templates& templates,
    bool bindInstantly)
{
    // Identify the needed parts of an entity
    tinyxml2::XMLElement* physic = nullptr;
    tinyxml2::XMLElement* shape = nullptr;

    sf::Vector2u pos(position);
    if(entity->Attribute("x") != nullptr)
        pos.x = entity->IntAttribute("x");
    if(entity->Attribute("y") != nullptr)
        pos.y = entity->IntAttribute("y");

    // Physics tag exists?
    physic = entity->FirstChildElement("physics");
    if(physic != nullptr)
    {
        // Shape template exists
        if(physic->Attribute("shape") != nullptr &&
            templates.shapes.find(std::string(physic->Attribute("shape"))) != end(templates.shapes))
            shape = templates.shapes.find(std::string(physic->Attribute("shape")))->second;
        // Physics doesn't use a template
        else
            shape = physic->FirstChildElement("shape");

        // Physics template exists other wise no template is used
        if(templates.physics.find(std::string(physic->Attribute("name"))) != end(templates.physics))
            physic = templates.physics.find(std::string(physic->Attribute("name")))->second;
    }

    return createEntity(entity, pos, shape, physic, templates, bindInstantly);
}

bool Level::validate(const tinyxml2::XMLDocument& document)
{
    if(document.Error()) // Error while loading file
    {
        document.PrintError();
        return false;
    }

    return true;

    // Check for required tags
    bool tagCheck = true;
    tagCheck &= (document.FirstChildElement("level") != nullptr);
    tagCheck &= (document.FirstChildElement("level")->FirstChildElement("objects") != nullptr);
    tagCheck &= (document.FirstChildElement("level")->FirstChildElement("world") != nullptr);
    tagCheck &= (document.FirstChildElement("level")->FirstChildElement("grid") != nullptr);
    tagCheck &= (document.FirstChildElement("level")->FirstChildElement("objects")
        ->FirstChildElement("grid") != nullptr);

    return tagCheck;
}

std::unique_ptr<Entity> Level::createEntity(
    tinyxml2::XMLElement* xml,
    const sf::Vector2u& position,
    tinyxml2::XMLElement* shape,
    tinyxml2::XMLElement* physic,
    Templates& templates,
    bool bindInstantly)
{
    std::unique_ptr<Entity> entity;
    tinyxml2::XMLElement* element = nullptr;
    
    if(xml->Attribute("type") != nullptr)
    {
        if(std::string(xml->Attribute("type")) == "teeter")
            entity = std::unique_ptr<Teeter>(new Teeter(m_config.get<float>("MouseScale")));
        else if(std::string(xml->Attribute("type")) == "ball")
            entity = std::unique_ptr<Ball>(new Ball(m_config.get<float>("BallResetTime")));
        else if(std::string(xml->Attribute("type")) == "target")
        {
            entity = std::unique_ptr<Entity>(new Entity(Entity::Target));
            m_totalTarget++;
        }
        else
            entity = std::unique_ptr<Entity>(new Entity(Entity::None));
    }
    else // No type specified == normal Entity
        entity = std::unique_ptr<Entity>(new Entity(Entity::None));

    entity->setName(std::string(xml->Attribute("name")));
    if(auto animations = xml->FirstChildElement("animations"))
    {
        // Load animation
        for(auto element = animations->FirstChildElement("animation"); element != nullptr;
            element = element->NextSiblingElement("animation"))
        {
            auto animation = LevelFileLoader::parseAnimation(element, entity.get(), this, m_resourceManager, &templates.functions);
            if(physic == nullptr)
                animation->setPosition(static_cast<float>(position.x), static_cast<float>(position.y));
            entity->bindAnimation(std::move(animation));
        }
    }
    if(auto constants = xml->FirstChildElement("constants"))
        LevelFileLoader::parseConstants(constants, entity.get());

    if(physic != nullptr)
    {
        if(xml->Attribute("collideWithBall") != nullptr)
            entity->setCollideWithBall(xml->BoolAttribute("collideWithBall"));
        else
            entity->setCollideWithBall(true);

        if(auto collider = xml->FirstChildElement("onCollision"))
            parseCollider(entity.get(), collider, templates);

        if(auto filter = xml->FirstChildElement("collides"))
            parseCollisionFilter(entity.get(), filter, templates);

        // Load sound
        if(xml->FirstChildElement("sound") != nullptr)
            entity->bindSound(std::string(xml->FirstChildElement("sound")->Attribute("name")), &m_soundManager);

        // Load body
        element = physic->FirstChildElement("body");
        b2BodyDef bodyDef;
        if(std::string(element->Attribute("type")) == "static")
            bodyDef.type = b2_staticBody;
        else if(std::string(element->Attribute("type")) == "kinematic")
            bodyDef.type = b2_kinematicBody;
        else if(std::string(element->Attribute("type")) == "dynamic")
            bodyDef.type = b2_dynamicBody;
        bodyDef.position = b2Vec2(static_cast<float>(utility::toMeter(position.x)), static_cast<float>(utility::toMeter(position.y)));
        bodyDef.angle = utility::toRadian<float, float>(element->FloatAttribute("angle"));
        bodyDef.fixedRotation = element->BoolAttribute("fixedRotation");
        bodyDef.angularDamping = element->BoolAttribute("angularDamping");
        LevelFileLoader::parseKinematics(element, entity.get(), this, &templates.functions);

        // Load shape
        if(std::string(shape->Attribute("type")) == "polygon") // Load polygon
        {
            std::vector<b2Vec2> vertices;

            // Iterate over the vertices
            for(tinyxml2::XMLElement* vertexIterator = shape->FirstChildElement("vertex");
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
        else if(std::string(shape->Attribute("type")) == "circle") // Load circle
        {
            std::unique_ptr<b2CircleShape> cs(new b2CircleShape);
            cs->m_radius = utility::toMeter(shape->FloatAttribute("radius"));
            m_shapes.push_back(std::move(cs));
        }

        // Load fixtures
        element = physic->FirstChildElement("fixture");
        b2FixtureDef fixtureDef;
        fixtureDef.shape = m_shapes.back().get();
        fixtureDef.density = element->FloatAttribute("density");
        fixtureDef.friction = element->FloatAttribute("friction");
        fixtureDef.restitution = element->FloatAttribute("restitution");

        entity->bindDefs(fixtureDef, bodyDef, &m_world);
        if(bindInstantly)
            entity->bindBody();
    }

    return std::move(entity);
}

void Level::parseCollider(
    Entity* entity,
    tinyxml2::XMLElement* xml,
    Templates& templates)
{
    for(auto child = xml->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        if(std::string(child->Name()) == "changeProperty")
        {
            std::unique_ptr<ChangePropertyCollisionHandler> collider(new ChangePropertyCollisionHandler(child->Attribute("name"), this));
            std::unique_ptr<ValueProvider> provider(LevelFileLoader::parseProvider(child->FirstChildElement(), collider.get(), collider.get(), &templates.functions));
            collider->bindProvider(std::move(provider));
            entity->bindCollisionHandler(std::move(collider));
        }
        else
            throw std::runtime_error(utility::replace(utility::translateKey("UnknownCollider"), child->Name()));
    }
}

std::unique_ptr<CollisionFilter> Level::getCollisionFilter(
    Entity* entity,
    tinyxml2::XMLElement* xml,
    Templates& templates)
{
    if(std::string(xml->Name()) == "always")
    {
        return std::unique_ptr<CollisionFilter>(new Always());
    }
    else if(std::string(xml->Name()) == "never")
    {
        return std::unique_ptr<CollisionFilter>(new Never());
    }
    else if(std::string(xml->Name()) == "spawnEntity")
    {
        auto name = xml->Attribute("name");
        if(!name)
            throw std::runtime_error(utility::translateKey("SpawnWithoutName"));

        std::unique_ptr<Entity> spawned(parseEntityFromTemplate(name, templates, sf::Vector2u(0, 0), false));
        if(spawned == nullptr)
            throw std::runtime_error(utility::replace(utility::translateKey("UnknownEntityName"), name));

        std::unique_ptr<CollisionFilter> subFilter = getCollisionFilter(entity, xml->FirstChildElement(), templates);

        std::unique_ptr<SpawnEntity> filter(new SpawnEntity([this](const Entity* owner, const Entity* spawned)
        {
            for(auto it = std::begin(m_unspawnedEntities); it != std::end(m_unspawnedEntities); ++it)
            {
                if(it->get() == spawned)
                {
                    auto e = std::move(*it);
                    m_unspawnedEntities.erase(it);
                    e->setPosition(owner->getPosition());
                    m_entitiesToSpawn.push_back(std::move(e));
                    break;
                }
            }
        }, entity, spawned.get(), std::move(subFilter)));
        m_unspawnedEntities.push_back(std::move(spawned));
        return std::move(filter);
    }
    else if(std::string(xml->Name()) == "changeGravity")
    {
        bool target = true;//std::string("entity") == child->Attribute("target");
        b2Vec2 gravity(xml->FloatAttribute("x"), xml->FloatAttribute("y"));
        std::unique_ptr<ChangeGravityFilter> filter(new ChangeGravityFilter(m_world, gravity, target, this));
        std::unique_ptr<ValueProvider> provider(LevelFileLoader::parseProvider(xml->FirstChildElement(), filter.get(), filter.get(), &templates.functions));
        filter->bindProvider(std::move(provider));
        return std::move(filter);
    }
    else if(std::string(xml->Name()) == "propertyFilter")
    {
        bool target = std::string("entity") == xml->Attribute("target");
        std::unique_ptr<PropertyFilter> filter(new PropertyFilter(target, this));
        std::unique_ptr<ValueProvider> provider(LevelFileLoader::parseProvider(xml->FirstChildElement(), filter.get(), filter.get(), &templates.functions));
        filter->bindProvider(std::move(provider));
        return std::move(filter);
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("UnknownFilter"), xml->Name()));
}

void Level::parseCollisionFilter(
    Entity* entity,
    tinyxml2::XMLElement* xml,
    Templates& templates)
{
    entity->bindCollisionFilter(getCollisionFilter(entity, xml->FirstChildElement(), templates));
}
