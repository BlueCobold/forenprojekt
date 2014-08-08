#include "Level.hpp"
#include "Teeter.hpp"

#include "../resources/AppConfig.hpp"
#include "../resources/LevelFileLoader.hpp"
#include "../Utility.hpp"

#include "collision/handler/ApplyImpulseCollisionHandler.hpp"
#include "collision/handler/BallTeleportCollisionHandler.hpp"
#include "collision/handler/ChangePropertyCollisionHandler.hpp"
#include "collision/handler/SpawnEntityCollisionHandler.hpp"
#include "collision/handler/GenericCollisionHandler.hpp"
#include "collision/filter/Always.hpp"
#include "collision/filter/ApplyForceFilter.hpp"
#include "collision/filter/ChangeBallSpawnFilter.hpp"
#include "collision/filter/ChangeBallVelocityFilter.hpp"
#include "collision/filter/ChangeGravityFilter.hpp"
#include "collision/filter/Never.hpp"
#include "collision/filter/PropertyFilter.hpp"
#include "collision/filter/SpawnEntity.hpp"
#include "../animation/provider/RandomProvider.hpp"

#include <cmath>

#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>

#include <tinyxml2.h>

void Level::load()
{
    if(m_number == 0) // Level start from 1
        throw std::runtime_error(utility::replace(utility::translateKey("InvalidLevelNumber"), filename()));

    tinyxml2::XMLDocument doc;
    doc.LoadFile(filename().c_str());

    if(!validate(doc)) // Validate the XML file
        throw std::runtime_error(utility::replace(utility::translateKey("InvalidXml"), filename()));

    // Parse templates
    Templates templates;
    std::vector<std::unique_ptr<tinyxml2::XMLDocument>> docs;
    parseTemplates(templates, doc.FirstChildElement("level"), docs);
    
    auto constants = doc.FirstChildElement("level")->FirstChildElement("constants");
    if(constants != nullptr)
        LevelFileLoader::parseConstants(constants, this);

    // get optional Attribute
    auto gameplay = doc.FirstChildElement("level")->FirstChildElement("gameplay");
    if(gameplay != nullptr)
        parseGameplayAttributes(gameplay);
    else
    {
        m_remainingBall = -1;
        m_remainingTime = -1.f;
        m_totalTime = -1.f;
        m_initialTime = -1.f;
    }
    // get Medal values
    m_bronzeMedal = doc.FirstChildElement("level")->FirstChildElement("medal")->IntAttribute("bronze");
    m_silverMedal = doc.FirstChildElement("level")->FirstChildElement("medal")->IntAttribute("silver");
    m_goldMedal = doc.FirstChildElement("level")->FirstChildElement("medal")->IntAttribute("gold");

    // get LevelName
    m_levelName = doc.FirstChildElement("level")->FirstChildElement("levelinfo")->Attribute("name");

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

    parseObjects(templates, doc.FirstChildElement("level"), docs);
    
    tinyxml2::XMLElement* world = doc.FirstChildElement("level")->FirstChildElement("world");
    // Load world properties
    tinyxml2::XMLElement* gravity = world->FirstChildElement("gravity");
    m_defaultGravity = b2Vec2(gravity->FloatAttribute("x"), gravity->FloatAttribute("y"));
    m_gravity = m_defaultGravity;
    m_world.SetContactListener(&m_contactListener);

    // setup scrollview
    m_scrollView.setLevelSize(sf::Vector2f(getWidth(), getHeight()));

    // get the fucking ball
    auto ballIt = end(m_entities);
    for(auto it = begin(m_entities); it != end(m_entities); ++it)
        if((*it)->getType() == Entity::Ball)
        {
            ballIt = it;
            m_ball = dynamic_cast<Ball*>((*it).get());
            m_ball->setFieldDimension(b2Vec2(m_width,m_height));
        }
    if(m_ball == nullptr)
        throw std::runtime_error("No ball located in the level!");

    // sort entity by her drawOrder
    std::sort(m_entities.begin(), m_entities.end(), compareDrawOrder);

    m_ball->registerForCheckpointChanges([this](){
        createLabelAt(m_ball, "green", "checkpoint");
    });

    m_remainingTarget = m_totalTarget;
}

void Level::parseObjects(
    Templates& templates,
    tinyxml2::XMLElement* root,
    std::vector<std::unique_ptr<tinyxml2::XMLDocument>>& docs)
{
    tinyxml2::XMLElement* objects = root->FirstChildElement("objects");
    if(objects == nullptr)
        return;

    // Load background-image
    if(tinyxml2::XMLElement* backgroundXml = objects->FirstChildElement("background"))
    {
        std::unique_ptr<Background> background(new Background(sf::Vector2u(
            static_cast<unsigned int>(m_width),
            static_cast<unsigned int>(m_height))));
        for(auto parallax = backgroundXml->FirstChildElement("parallax"); parallax != nullptr;
            parallax = parallax->NextSiblingElement("parallax"))
        {
            std::unique_ptr<ParallaxLayer> layer(new ParallaxLayer(sf::Vector2f(
                parallax->FloatAttribute("width"),
                parallax->FloatAttribute("height"))));
            for(auto anim = parallax->FirstChildElement("animation"); anim != nullptr;
                anim = anim->NextSiblingElement("animation"))
                layer->bindAnimation(std::move(LevelFileLoader::parseAnimation(anim, layer.get(), nullptr, m_resourceManager, &templates.functions)));
            background->bindLayer(std::move(layer));
        }
        m_background = std::move(background);
    }

    for(tinyxml2::XMLElement* entitiesIterator = objects->FirstChildElement("entity");
        entitiesIterator != nullptr; entitiesIterator = entitiesIterator->NextSiblingElement("entity"))
    {
        m_entities.push_back(std::move(parseEntity(entitiesIterator, sf::Vector2u(0, 0), templates)));
    }
   
    for(auto child = objects->FirstChildElement("include"); child != nullptr; child = child->NextSiblingElement("include"))
    {
        std::unique_ptr<tinyxml2::XMLDocument> doc(new tinyxml2::XMLDocument);
        std::string filename = pathname() + child->Attribute("file");
        doc->LoadFile(filename.c_str());            
        if(!validate(*(doc.get())))
            throw std::runtime_error(utility::replace(utility::translateKey("InvalidXml"), filename));
        parseTemplates(templates, doc->RootElement(), docs);
        parseObjects(templates, doc->RootElement(), docs);
        docs.push_back(std::move(doc));
    }
}

void Level::parseTemplates(
    Templates& templates,
    tinyxml2::XMLElement* root,
    std::vector<std::unique_ptr<tinyxml2::XMLDocument>>& docs)
{
    tinyxml2::XMLElement* xmlTemplates = root->FirstChildElement("templates");
    if(xmlTemplates == nullptr)
        return;

    if(tinyxml2::XMLElement* shapes = xmlTemplates->FirstChildElement("shapes"))
    {
        auto values = std::move(LevelFileLoader::parseList(shapes, "shape", "name"));
        templates.shapes.insert(begin(values), end(values));
    }

    if(tinyxml2::XMLElement* physics = xmlTemplates->FirstChildElement("physics"))
    {
        auto values = std::move(LevelFileLoader::parseList(physics, "physic", "name"));
        templates.physics.insert(begin(values), end(values));
    }

    if(tinyxml2::XMLElement* functions = xmlTemplates->FirstChildElement("functions"))
    {
        auto values = std::move(LevelFileLoader::parseList(functions, "function", "name"));
        templates.functions.insert(begin(values), end(values));
    }

    if(tinyxml2::XMLElement* entities = xmlTemplates->FirstChildElement("entities"))
    {
        // Add use keys 'name' (objects) and 'rep' (grid)
        auto reps = std::move(LevelFileLoader::parseList(entities, "entity", "rep"));
        templates.entities.insert(begin(reps), end(reps));
        auto temp = std::move(LevelFileLoader::parseList(entities, "entity", "name"));
        templates.entities.insert(begin(temp), end(temp));
    }

    if(tinyxml2::XMLElement* functions = xmlTemplates->FirstChildElement("overrides"))
    {
        auto values = std::move(LevelFileLoader::parseList(functions, "override", "newRep"));
        templates.overrides.insert(begin(values), end(values));
        values = std::move(LevelFileLoader::parseList(functions, "override", "newName"));
        templates.overrides.insert(begin(values), end(values));
    }

    for(auto child = xmlTemplates->FirstChildElement("include"); child != nullptr; child = child->NextSiblingElement("include"))
    {
        std::unique_ptr<tinyxml2::XMLDocument> doc(new tinyxml2::XMLDocument);
        std::string filename = pathname() + child->Attribute("file");
        doc->LoadFile(filename.c_str());            
        if(!validate(*(doc.get())))
            throw std::runtime_error(utility::replace(utility::translateKey("InvalidXml"), filename));
        parseTemplates(templates, doc->RootElement(), docs);
        parseObjects(templates, doc->RootElement(), docs);
        docs.push_back(std::move(doc));
    }
}

std::string buildOriginal(const std::string& targetName, const std::string& sourceName, const std::string& needle)
{
    if(targetName == needle)
        return sourceName;
    if(targetName.length() != 2 || sourceName.length() != 2 || needle.length() != 2 ||(targetName[0] != '*' && targetName[1] != '*'))
        return needle;
    else if(targetName[0] == '*' && (targetName[1] == needle[1] || targetName[1] == '*'))
        return std::string(1, needle[0]) + sourceName[1];
    else if(targetName[1] == '*' && (targetName[0] == needle[0] || targetName[0] == '*'))
        return std::string(1, sourceName[0]) + needle[1];
    return "";
}

std::unique_ptr<Entity> Level::parseEntityFromTemplate(
    std::string name,
    Templates& templates,
    sf::Vector2u& position,
    bool bindInstantly)
{
    auto match = templates.entities.find(name);
    if(match == end(templates.entities))
    {
        for(auto it = begin(templates.overrides); it != end(templates.overrides); ++it)
        {
            std::string originalName;
            if(auto xmlName = it->second->Attribute("rep"))
                originalName = buildOriginal(it->first, xmlName, name);
            if(originalName.empty() || originalName == name)
            {
                if(auto oldName = it->second->Attribute("name"))
                    if(it->first == name)
                        originalName = oldName;
            }
            if(originalName.empty() || originalName == name)
                continue;

            match = templates.entities.find(originalName);
            std::unique_ptr<Entity> original;

            if(it->second->Attribute("offsetx") != nullptr)
                position.x += it->second->IntAttribute("offsetx");
            if(it->second->Attribute("offsety") != nullptr)
                position.y += it->second->IntAttribute("offsety");

            // found some original?
            if(match != end(templates.entities))
                original = parseEntity(match->second, position, templates, bindInstantly);
            else
                // maybe the parent is also an override? Re-test.
                original = parseEntityFromTemplate(originalName, templates, position, bindInstantly);
            if(original == nullptr)
                return nullptr;
            auto xml = it->second;
            auto entity = original.get();
            if(it->second->Attribute("z") != nullptr)
                entity->setDrawOrder(it->second->FloatAttribute("z"));
            if(auto constantsXml = xml->FirstChildElement("constants"))
                LevelFileLoader::parseConstants(constantsXml, entity);
            if(original->hasPhysics())
            {
                if(auto collider = xml->FirstChildElement("onCollision"))
                    parseCollider(entity, collider, templates);
                if(auto filter = xml->FirstChildElement("collides"))
                    parseCollisionFilter(entity, filter, templates);
            }
            if(auto aniXml = xml->FirstChildElement("animations"))
            {
                entity->applyOverrides([&](Animation* animation)
                {
                    LevelFileLoader::parseColorController(animation, aniXml, entity, this, &templates.functions);
                    LevelFileLoader::parsePositionController(animation, aniXml, entity, this, &templates.functions);
                    LevelFileLoader::parseScaleController(animation, aniXml, entity, this, &templates.functions);
                    LevelFileLoader::parseRotationController(animation, aniXml, entity, this, &templates.functions);
                    if(auto setXml = aniXml->FirstChildElement("set"))
                    {
                        if(setXml->Attribute("rotate"))
                            animation->applyRotation(setXml->BoolAttribute("rotate"));
                    }
                    if(auto constants = aniXml->FirstChildElement("constants"))
                        LevelFileLoader::parseConstants(constants, animation);
                });
            }
            if(auto kill = parseEntityReference("onKill", xml, templates))
            {
                entity->bindKillAnimationEntity(kill.get());
                m_unspawnedEntities.push_back(EntitySpawn(std::move(kill)));
            }
            if(auto name = xml->Attribute("newName"))
                entity->setName(name);

            tinyxml2::XMLElement* physic = nullptr;
            tinyxml2::XMLElement* shape = nullptr;
            findPhysicAndShapeTag(physic, shape, xml, templates);
            if(auto draworder = it->second->FloatAttribute("draworder"))
                entity->setDrawOrder(draworder);
            if(physic != nullptr)
                parsePhysics(physic, shape, entity, position, templates);
            return std::move(original);
        }
        return nullptr;
    }

    return parseEntity(match->second, position, templates, bindInstantly);
}

void Level::findPhysicAndShapeTag(tinyxml2::XMLElement*& physic, tinyxml2::XMLElement*& shape, 
    tinyxml2::XMLElement* entity,
    Templates& templates)
{
    auto physics = entity->FirstChildElement("physics");
    if(physics != nullptr)
    {
        // Shape template exists
        if(physics->Attribute("shape") != nullptr)
        {
            std::string name(physics->Attribute("shape"));
            if(templates.shapes.find(name) != end(templates.shapes))
                shape = templates.shapes.find(std::string(physics->Attribute("shape")))->second;
        }
        // Physics doesn't use a template
        else
            shape = physics->FirstChildElement("shape");

        if(physics->Attribute("name"))
        {
            auto name = std::string(physics->Attribute("name"));
            // Physics template exists other wise no template is used
            if(templates.physics.find(std::string(physics->Attribute("name"))) != end(templates.physics))
                physic = templates.physics.find(name)->second;
            if(physic == nullptr)
                throw std::runtime_error(utility::replace(utility::translateKey("UnknownPhysicReference"), name));
        }
        else
            physic = physics;
    }
}

std::unique_ptr<Entity> Level::parseEntity(
    tinyxml2::XMLElement* entity,
    sf::Vector2u& position,
    Templates& templates,
    bool bindInstantly)
{

    if(entity->Attribute("x") != nullptr)
        position.x = entity->IntAttribute("x");
    if(entity->Attribute("y") != nullptr)
        position.y = entity->IntAttribute("y");
    if(entity->Attribute("offsetx") != nullptr)
        position.x += entity->IntAttribute("offsetx");
    if(entity->Attribute("offsety") != nullptr)
        position.y += entity->IntAttribute("offsety");

    // Identify the needed parts of an entity
    tinyxml2::XMLElement* physic = nullptr;
    tinyxml2::XMLElement* shape = nullptr;
    findPhysicAndShapeTag(physic, shape, entity, templates);

    if(entity->Attribute("z") != nullptr)
    {
        std::unique_ptr<Entity> graphicalObject = createEntity(entity, position, shape, physic, templates, bindInstantly);
        graphicalObject.get()->setDrawOrder(entity->FloatAttribute("z"));

        return graphicalObject;
    }

    return createEntity(entity, position, shape, physic, templates, bindInstantly);
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

void Level::parsePhysics(tinyxml2::XMLElement* physic,
    tinyxml2::XMLElement* shape,
    Entity* entity,
    const sf::Vector2u& position,
    Templates& templates,
    bool isBullet)
{
    b2BodyDef bodyDef;
    LevelFileLoader::parseBodyDef(physic, entity, this, &templates.functions, bodyDef, position);
    bodyDef.bullet = isBullet;

    std::vector<std::unique_ptr<b2Shape>> shapes;
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
        shapes.push_back(std::move(ps));
    }
    else if(std::string(shape->Attribute("type")) == "complex_polygon") // Load polygon
    {
        for(auto polyIterator = shape->FirstChildElement("polygon");
            polyIterator != nullptr; polyIterator = polyIterator->NextSiblingElement("polygon"))
        {
            std::vector<b2Vec2> vertices;
            // Iterate over the vertices
            for(auto vertexIterator = polyIterator->FirstChildElement("vertex");
                vertexIterator != nullptr; vertexIterator = vertexIterator->NextSiblingElement("vertex"))
            {
                vertices.push_back(b2Vec2(utility::toMeter(vertexIterator->FloatAttribute("x")),
                    utility::toMeter(vertexIterator->FloatAttribute("y"))));
            }
            // Construct the b2Shape
            std::unique_ptr<b2PolygonShape> ps(new b2PolygonShape);
            ps->Set(vertices.data(), vertices.size());
            shapes.push_back(std::move(ps));
        }
    }
    else if(std::string(shape->Attribute("type")) == "circle") // Load circle
    {
        std::unique_ptr<b2CircleShape> cs(new b2CircleShape);
        cs->m_radius = utility::toMeter(shape->FloatAttribute("radius"));
        shapes.push_back(std::move(cs));
    }

    // Load fixtures
    auto fixtureXml = physic->FirstChildElement("fixture");
    b2FixtureDef fixtureDef;
    //fixtureDef.shape = &shapes.g m_shapes.back().get();
    fixtureDef.density = fixtureXml->FloatAttribute("density");
    fixtureDef.friction = fixtureXml->FloatAttribute("friction");
    fixtureDef.restitution = fixtureXml->FloatAttribute("restitution");

    entity->bindDefs(fixtureDef, shapes, bodyDef, &m_world);
}

std::unique_ptr<Entity> Level::createEntity(
    tinyxml2::XMLElement* xml,
    sf::Vector2u& position,
    tinyxml2::XMLElement* shape,
    tinyxml2::XMLElement* physic,
    Templates& templates,
    bool bindInstantly)
{
    std::unique_ptr<Entity> entity;
    
    bool respawnable = xml->BoolAttribute("respawnable");
    bool autoStop = xml->BoolAttribute("stopWithLastAnimation");

    bool isBullet = false;
    if(auto name = xml->Attribute("base"))
        entity = parseEntityFromTemplate(name, templates, position, false);
    else
    {
        std::string typeName;
        if(xml->Attribute("type") != nullptr)
            typeName = xml->Attribute("type");

        if(typeName == "teeter")
            entity = std::unique_ptr<Teeter>(new Teeter(m_config.get<float>("MouseScale")));
        else if(typeName == "ball")
        {
            isBullet = true;
            std::unique_ptr<Entity> spawn = parseEntityReference("onRespawn", xml, templates);
            std::unique_ptr<Entity> kill = parseEntityReference("onKill", xml, templates);
            float autoKillSpeed = xml->FloatAttribute("autokillspeed");
            auto ball = new Ball(m_config.get<float>("BallResetTime"), autoKillSpeed, spawn.get(), kill.get());
            ball->bindTrail(LevelFileLoader::parseTrail(ball, xml, m_resourceManager, &templates.functions));
            entity = std::unique_ptr<Ball>(ball);
            if(spawn != nullptr)
                m_unspawnedEntities.push_back(EntitySpawn(std::move(spawn)));
            if(kill != nullptr)
                m_unspawnedEntities.push_back(EntitySpawn(std::move(kill)));
        }
        else if(typeName == "target")
        {
            entity = std::unique_ptr<Entity>(new Entity(Entity::Target, respawnable, autoStop));
            m_normalTargetPoints = xml->IntAttribute("points");
            if(m_normalTargetPoints == 0)
                m_normalTargetPoints = 100;
            m_totalTarget++;
        }
        else if(typeName == "bonustarget")
        {
            entity = std::unique_ptr<Entity>(new Entity(Entity::BonusTarget, respawnable, autoStop));
            m_bonusTargetPoints = xml->IntAttribute("points");
            if(m_bonusTargetPoints == 0)
                m_bonusTargetPoints = 10;
        }
        else // No type or unknown type specified => normal Entity
        {
            entity = std::unique_ptr<Entity>(new Entity(Entity::None, respawnable, autoStop));
            if(auto kill = parseEntityReference("onKill", xml, templates))
            {
                entity->bindKillAnimationEntity(kill.get());
                m_unspawnedEntities.push_back(EntitySpawn(std::move(kill)));
            }
        }
    }

    entity->setName(std::string(xml->Attribute("name")));
    // time to set new draworder
    if(xml->Attribute("draworder") != nullptr)
        entity->setDrawOrder(xml->FloatAttribute("draworder"));

    if(auto animations = xml->FirstChildElement("animations"))
    {
        // Load animation
        for(auto element = animations->FirstChildElement("animation"); element != nullptr;
            element = element->NextSiblingElement("animation"))
        {
            int copies = 1;
            element->QueryIntAttribute("copies", &copies);
            for(int copy = 0; copy < copies; copy++)
            {
                auto animation = LevelFileLoader::parseAnimation(element, entity.get(), this, m_resourceManager, &templates.functions);
                if(physic == nullptr)
                    entity->setPosition(
                        b2Vec2(
                            static_cast<float>(utility::toMeter(position.x)),
                            static_cast<float>(utility::toMeter(position.y))));
                entity->bindAnimation(std::move(animation));
            }
        }
    }
    if(auto constants = xml->FirstChildElement("constants"))
        LevelFileLoader::parseConstants(constants, entity.get());

    // Load sound
    if(xml->FirstChildElement("sound") != nullptr)
    {
        auto sound = std::unique_ptr<SoundObject>(new SoundObject(std::string(xml->FirstChildElement("sound")->Attribute("name")), m_resourceManager.getSoundManager()));
        if(xml->FirstChildElement("sound")->Attribute("volume"))
            sound->fixVolume(xml->FirstChildElement("sound")->FloatAttribute("volume"));
        entity->bindCollisionSound(std::move(sound));
    }
    if(xml->FirstChildElement("sounds") != nullptr)
    {
        std::vector<std::unique_ptr<SoundTrigger>> otherSounds;
        for(auto sound = xml->FirstChildElement("sounds")->FirstChildElement("sound"); sound != nullptr; sound = sound->NextSiblingElement())
        {
            std::string soundName = sound->Attribute("name");
            std::unique_ptr<ValueProvider> provider(LevelFileLoader::parseProvider(sound->FirstChildElement(), nullptr, nullptr, nullptr, &templates.functions));
            otherSounds.push_back(std::unique_ptr<SoundTrigger>(new SoundTrigger(soundName, m_resourceManager.getSoundManager(), std::move(provider))));
        }
        entity->bindOtherSounds(std::move(otherSounds));
    }
    if(physic != nullptr)
        parsePhysics(physic, shape, entity.get(), position, templates, isBullet);
    
    if(auto collider = xml->FirstChildElement("onCollision"))
        parseCollider(entity.get(), collider, templates);

    if(entity->hasPhysics())
    {
        if(xml->Attribute("collideWithBall") != nullptr)
            entity->setCollideWithBall(xml->BoolAttribute("collideWithBall"));
        else
            entity->setCollideWithBall(true);

        if(auto filter = xml->FirstChildElement("collides"))
            parseCollisionFilter(entity.get(), filter, templates);

        if(bindInstantly)
            entity->bindBody();
    }

    return std::move(entity);
}

enum SpawnLocation
{
    BallLoc = 1,
    OwnerLoc = 2,
    ContactPoint = 3
};

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
            std::unique_ptr<ValueProvider> provider(LevelFileLoader::parseProvider(child->FirstChildElement(), collider.get(), collider.get(), nullptr, &templates.functions));
            collider->bindProvider(std::move(provider));
            entity->bindCollisionHandler(std::move(collider));
        }
        else if(std::string(child->Name()) == "applyImpulse")
        {
            std::unique_ptr<ApplyImpulseCollisionHandler> collider(new ApplyImpulseCollisionHandler(child->FloatAttribute("x"), child->FloatAttribute("y")));
            entity->bindCollisionHandler(std::move(collider));
        }
        else if(std::string(child->Name()) == "ballTeleport")
        {
            std::unique_ptr<BallTeleportCollisionHandler> collider(new BallTeleportCollisionHandler(child->FloatAttribute("x"), child->FloatAttribute("y"), !child->BoolAttribute("keepSpeed")));
            entity->bindCollisionHandler(std::move(collider));
        }
        else if(std::string(child->Name()) == "bonusTime")
        {
            int bonusTime = child->IntAttribute("value");
            std::unique_ptr<GenericCollisionHandler> collider(new GenericCollisionHandler(
            [=](Entity* entityA, Entity* entityB, const b2Vec2& point, const float impulse)
            {
                m_remainingTime += static_cast<float>(bonusTime);
            }));
            entity->bindCollisionHandler(std::move(collider));
        }
        else if(std::string(child->Name()) == "killBall")
        {
            std::unique_ptr<GenericCollisionHandler> collider(new GenericCollisionHandler(
            [&](Entity* entityA, Entity* entityB, const b2Vec2& point, const float impulse)
            {
                if(m_invulnerableGoody.isActive())
                    return;
                if(entityA->getType() != Entity::Ball && entityB->getType() != Entity::Ball)
                    throw std::runtime_error(utility::replace(utility::translateKey("EntityNoCollision"), "Ball"));
                m_ball->blowUp();
                if(m_ball->getKillAnimationEntity() != nullptr)
                    prepareEntityForSpawn(m_ball->getPosition(), m_ball->getKillAnimationEntity());
            }));
            entity->bindCollisionHandler(std::move(collider));
        }
        else if(std::string(child->Name()) == "showLabel")
        {
            std::unique_ptr<CollisionHandler> collider = parseShowLabelHandler(child);
            entity->bindCollisionHandler(std::move(collider));
        }
        else if(std::string(child->Name()) == "spawnEntity")
        {
            auto name = child->Attribute("name");
            if(!name)
                throw std::runtime_error(utility::translateKey("SpawnWithoutName"));

            std::unique_ptr<Entity> spawned(parseEntityFromTemplate(name, templates, sf::Vector2u(0, 0), false));
            if(spawned == nullptr)
                throw std::runtime_error(utility::replace(utility::translateKey("UnknownEntityName"), name));

            SpawnLocation location = BallLoc;
            auto loc = child->Attribute("location");
            if(loc && std::string("contact") == loc)
                location = ContactPoint;
            else if(loc && std::string("entity") == loc)
                location = OwnerLoc;

            std::unique_ptr<SpawnEntityCollisionHandler> handler(new SpawnEntityCollisionHandler(
            [=](const Entity* owner, const Entity* spawned, const b2Vec2& contactPoint)
            {
                switch(location)
                {
                    case BallLoc:
                        prepareEntityForSpawn(m_ball->getPosition(), spawned);
                        break;
                    case OwnerLoc:
                        prepareEntityForSpawn(owner->getPosition(), spawned);
                        break;
                    case ContactPoint:
                        prepareEntityForSpawn(contactPoint, spawned);
                        break;
                }
            }, entity, spawned.get()));
            m_unspawnedEntities.push_back(EntitySpawn(std::move(spawned)));
            entity->bindCollisionHandler(std::move(handler));
        }
        else
            throw std::runtime_error(utility::replace(utility::translateKey("UnknownCollider"), child->Name()));
    }
}

std::unique_ptr<CollisionHandler> Level::parseShowLabelHandler(tinyxml2::XMLElement* xml)
{
    std::string font(xml->Attribute("font"));
    auto distance = xml->FloatAttribute("offset");
    
    std::vector<std::string> labels;
    for(auto child = xml->FirstChildElement("label"); child != nullptr; child = child->NextSiblingElement("label"))
        labels.push_back(utility::translateKey(child->GetText()));

    float time = -1;
    return std::unique_ptr<GenericCollisionHandler>(
        new GenericCollisionHandler([=](Entity* entityA, Entity* entityB, const b2Vec2& point, const float impulse)
        mutable {
            // prevent a spam of labels... only one per second per entity!
            if(time > getPassedTime())
                return;

            time = getPassedTime() + 1;
            RandomProvider lengthRandom(0.f, distance);
            RandomProvider angleRandom(0.f, utility::toRadian<float, float>(360.f));
            RandomProvider textIndex(0.f, static_cast<float>(labels.size()));

            std::string text = labels[static_cast<int>(textIndex.getValue())];
            auto angle = angleRandom.getValue();
            auto length = lengthRandom.getValue();

            auto pos = sf::Vector2f(
                utility::toPixel(entityB->getPosition().x), 
                utility::toPixel(entityB->getPosition().y));
            sf::Vector2f offset(length * sinf(angle), length * cosf(angle));

            this->createLabelAt(offset + pos, font, text);
        }));
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
            prepareEntityForSpawn(owner->getPosition(), spawned);
        }, entity, spawned.get(), std::move(subFilter)));
        m_unspawnedEntities.push_back(EntitySpawn(std::move(spawned)));
        return std::move(filter);
    }
    else if(std::string(xml->Name()) == "copySpawnLocation")
    {
        std::unique_ptr<CollisionFilter> subFilter = getCollisionFilter(entity, xml->FirstChildElement(), templates);
        std::unique_ptr<ChangeBallSpawnFilter> filter(new ChangeBallSpawnFilter(std::move(subFilter)));
        return std::move(filter);
    }
    else if(std::string(xml->Name()) == "setBallVelocity")
    {
        std::unique_ptr<CollisionFilter> subFilter = getCollisionFilter(entity, xml->FirstChildElement(), templates);
        std::unique_ptr<ChangeBallVelocityFilter> filter(new ChangeBallVelocityFilter(
            xml->FloatAttribute("x"),
            xml->FloatAttribute("y"),
            std::move(subFilter)));
        return std::move(filter);
    }
    else if(std::string(xml->Name()) == "applyForce")
    {
        b2Vec2 force(xml->FloatAttribute("x"), xml->FloatAttribute("y"));
        std::unique_ptr<CollisionFilter> subFilter = getCollisionFilter(entity, xml->FirstChildElement(), templates);
        std::unique_ptr<ApplyForceFilter> filter(new ApplyForceFilter(force, std::move(subFilter)));
        return std::move(filter);
    }
    else if(std::string(xml->Name()) == "changeGravity")
    {
        bool target = true;//std::string("entity") == child->Attribute("target");
        b2Vec2 gravity(xml->FloatAttribute("x"), xml->FloatAttribute("y"));
        std::unique_ptr<ChangeGravityFilter> filter(new ChangeGravityFilter(m_gravity, gravity, target, this));
        std::unique_ptr<ValueProvider> provider(LevelFileLoader::parseProvider(xml->FirstChildElement(), filter.get(), filter.get(), nullptr, &templates.functions));
        filter->bindProvider(std::move(provider));
        return std::move(filter);
    }
    else if(std::string(xml->Name()) == "propertyFilter")
    {
        bool target = std::string("entity") == xml->Attribute("target");
        std::unique_ptr<PropertyFilter> filter(new PropertyFilter(target, this));
        std::unique_ptr<ValueProvider> provider(LevelFileLoader::parseProvider(xml->FirstChildElement(), filter.get(), filter.get(), nullptr, &templates.functions));
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

std::unique_ptr<Entity> Level::parseEntityReference(
    const std::string& key,
    tinyxml2::XMLElement* xml,
    Templates& templates)
{
    auto spawn = xml->FirstChildElement(key.c_str());
    if(spawn == nullptr)
        return nullptr;
    auto action = spawn->FirstChildElement("spawnEntity");
    if(action == nullptr)
        return nullptr;
    auto name = action->Attribute("name");
    if(!name)
        throw std::runtime_error(utility::translateKey("SpawnWithoutName"));

    std::unique_ptr<Entity> spawned(parseEntityFromTemplate(name, templates, sf::Vector2u(0, 0), false));
    if(spawned == nullptr)
        throw std::runtime_error(utility::replace(utility::translateKey("UnknownEntityName"), name));
    return spawned;
}

void Level::prepareEntityForSpawn(const b2Vec2& position, const Entity* spawn, float angle)
{
    for(auto it = std::begin(m_unspawnedEntities); it != std::end(m_unspawnedEntities); ++it)
    {
        if(it->target.get() == spawn)
        {
            it->target->setPosition(position);
            it->target->setAnimationAngle(angle);
            m_entitiesToSpawn.push_back(std::move(it->target));
            m_unspawnedEntities.erase(it);
            //it->target.reset();
            break;
        }
    }
}

void Level::parseGameplayAttributes(tinyxml2::XMLElement* xml)
{
    int balls = xml->IntAttribute("maxBalls");
    float remainingTime = xml->FloatAttribute("time");
    if(balls > 0)
        m_remainingBall = balls;
    else
        m_remainingBall = -1;

    if(remainingTime > 0)
    {
        m_remainingTime = remainingTime;
        m_totalTime = m_remainingTime;
        m_initialTime = m_totalTime;
    }
    else
    {
        m_remainingTime = -1.f;
        m_totalTime = -1.f;
        m_initialTime = -1.f;
    }
}
