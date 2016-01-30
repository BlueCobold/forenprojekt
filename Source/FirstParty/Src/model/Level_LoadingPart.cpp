#include "Level.hpp"
#include "Teeter.hpp"
#include "EntityFactory.hpp"

#include "../resources/AnimationParser.hpp"
#include "../resources/AppConfig.hpp"
#include "../resources/ControllerParser.hpp"
#include "../resources/JointParser.hpp"
#include "../resources/LevelFileLoader.hpp"
#include "../resources/PhysicsParser.hpp"
#include "../resources/ValueParser.hpp"
#include "../Utility.hpp"
#include "../MacHelper.hpp"

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
#include "../rendering/Shader.hpp"

#include "joint/SingleRevoluteJoint.hpp"
#include "joint/SinglePrismaticJoint.hpp"
#include "joint/SingleDistanceJoint.hpp"

#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>

#include <tinyxml2.h>

#include <algorithm>

void Level::load()
{
#ifndef LEVELTESTING
    if(m_number == 0) // Level start from 1
        throw std::runtime_error(utility::replace(utility::translateKey("InvalidLevelNumber"), filename()));

    if(!isOriginal())
        throw std::runtime_error(utility::replace(utility::translateKey("NoOriginalFile"), filename()));

#endif
    tinyxml2::XMLDocument doc;

    std::string filePath;
#ifdef LEVELTESTING
    if(number() == 0)
        filePath = m_filename;
    else
#endif
    filePath = resourcePath() + filename();
    doc.LoadFile(filePath.c_str());

    if(!validate(doc)) // Validate the XML file
        throw std::runtime_error(utility::replace(utility::translateKey("InvalidXml"), filePath));

    // Parse templates
    Templates templates;
    std::vector<std::unique_ptr<tinyxml2::XMLDocument>> docs;
    parseTemplates(templates, doc.FirstChildElement("level"), docs);

    auto constants = doc.FirstChildElement("level")->FirstChildElement("constants");
    if(constants != nullptr)
        ValueParser::parseConstants(*constants, *this);

    // get optional Attribute
    if(auto gameplay = doc.FirstChildElement("level")->FirstChildElement("gameplay"))
        parseGameplayAttributes(gameplay);
    else
    {
        m_remainingBall = -1;
        m_remainingTime = -1.f;
        m_totalTime = -1.f;
        m_initialTime = -1.f;
    }
    if(auto setup = doc.FirstChildElement("level")->FirstChildElement("setup"))
        setup->QueryUnsignedAttribute("defaultBufferId", &m_defaultTargetBuffer);
    if(!Shader::isUsable())
    {
        if(auto setup = doc.FirstChildElement("level")->FirstChildElement("noShaderSetup"))
            setup->QueryUnsignedAttribute("defaultBufferId", &m_defaultTargetBuffer);
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
    std::vector<std::string> lines = LevelFileLoader::parseGrid(*grid);
    for(size_t row = 0; row < lines.size(); ++row)
        for(std::size_t column = 0; column < lines[row].length(); column += 2)
        {
            std::string name = lines[row].substr(column, 2);

            // Ignore empty 'tiles'
            if(name == "  ")
                continue;

            auto pos = sf::Vector2u(static_cast<unsigned int>((column / 2) * size), static_cast<unsigned int>(row * size));
            std::unique_ptr<Entity> entity = parseEntityFromTemplate(name, templates, pos);
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
    {
        if((*it)->getType() == Entity::Ball && ballIt == end(m_entities))
        {
            ballIt = it;
            m_ball = dynamic_cast<Ball*>((*it).get());
            m_ball->setFieldDimension(b2Vec2(m_width,m_height));
        }
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
    const tinyxml2::XMLElement* root,
    std::vector<std::unique_ptr<tinyxml2::XMLDocument>>& docs)
{
    auto objects = root->FirstChildElement("objects");
    if(objects == nullptr)
        return;

    // Load background-image
    if(auto backgroundXml = objects->FirstChildElement("background"))
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
            {
                auto context = ProviderContext(nullptr, layer.get(), layer.get(), layer.get(), m_cloneHandler)
                                                    .withFunctions(templates.functions);
                AnimationParser loader(context, m_resourceManager);
                if(auto animation = loader.parseSingle(*anim))
                {
                    if(animation->getBufferId() == UINT_MAX)
                        animation->setBufferId(m_defaultTargetBuffer);
                    layer->bindAnimation(std::move(animation));
                }
            }
            background->bindLayer(std::move(layer));
        }
        m_background = std::move(background);
    }

    for(auto entitiesIterator = objects->FirstChildElement("entity");
        entitiesIterator != nullptr; entitiesIterator = entitiesIterator->NextSiblingElement("entity"))
    {
        auto pos = sf::Vector2u(0, 0);
        m_entities.push_back(std::move(parseEntity(entitiesIterator, pos, templates)));
    }
   
    for(auto child = objects->FirstChildElement("include"); child != nullptr; child = child->NextSiblingElement("include"))
    {
        std::unique_ptr<tinyxml2::XMLDocument> doc(new tinyxml2::XMLDocument);
        std::string filename = resourcePath() + pathname() + child->Attribute("file");
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
    const tinyxml2::XMLElement* root,
    std::vector<std::unique_ptr<tinyxml2::XMLDocument>>& docs)
{
    auto xmlTemplates = root->FirstChildElement("templates");
    if(xmlTemplates == nullptr)
        return;

    if(auto shapes = xmlTemplates->FirstChildElement("shapes"))
    {
        auto values = std::move(ValueParser::parseList(*shapes, "shape", "name"));
        templates.shapes.insert(begin(values), end(values));
    }

    if(auto physics = xmlTemplates->FirstChildElement("physics"))
    {
        auto values = std::move(ValueParser::parseList(*physics, "physic", "name"));
        templates.physics.insert(begin(values), end(values));
    }

    if(auto functions = xmlTemplates->FirstChildElement("functions"))
    {
        auto values = std::move(ValueParser::parseList(*functions, "function", "name"));
        templates.functions.insert(begin(values), end(values));
    }

    if(auto entities = xmlTemplates->FirstChildElement("entities"))
    {
        // Add use keys 'name' (objects) and 'rep' (grid)
        auto reps = std::move(ValueParser::parseList(*entities, "entity", "rep"));
        templates.entities.insert(begin(reps), end(reps));
        auto temp = std::move(ValueParser::parseList(*entities, "entity", "name"));
        templates.entities.insert(begin(temp), end(temp));
    }

    if(auto functions = xmlTemplates->FirstChildElement("overrides"))
    {
        auto values = std::move(ValueParser::parseList(*functions, "override", "newRep"));
        templates.overrides.insert(begin(values), end(values));
        values = std::move(ValueParser::parseList(*functions, "override", "newName"));
        templates.overrides.insert(begin(values), end(values));
    }

    for(auto child = xmlTemplates->FirstChildElement("include"); child != nullptr; child = child->NextSiblingElement("include"))
    {
        std::unique_ptr<tinyxml2::XMLDocument> doc(new tinyxml2::XMLDocument);
        std::string filename = resourcePath() + pathname() + child->Attribute("file");
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
                ValueParser::parseConstants(*constantsXml, *entity);

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
                    auto context = ProviderContext(this, entity, entity, entity, m_cloneHandler).withFunctions(templates.functions);

                    ControllerParser controllerParser(context);
                    auto position = controllerParser.parsePosition(*aniXml);
                    animation->bindPositionController(std::move(*position[0]), std::move(*position[1]));

                    auto scale = controllerParser.parseScale(*aniXml);
                    animation->bindScaleController(std::move(*scale[0]), std::move(*scale[1]));

                    auto color = controllerParser.parseColor(*aniXml);
                    animation->bindColorController(std::move(*color[0]), std::move(*color[1]), std::move(*color[2]), std::move(*color[3]));

                    animation->bindRotationController(controllerParser.parseRotation(*aniXml));

                    if(auto setXml = aniXml->FirstChildElement("set"))
                    {
                        if(setXml->Attribute("rotate"))
                            animation->applyRotation(setXml->BoolAttribute("rotate"));
                    }
                    if(auto constants = aniXml->FirstChildElement("constants"))
                        ValueParser::parseConstants(*constants, *animation);
                });
            }

            if(auto kill = parseEntityReference("onKill", xml, templates))
            {
                entity->bindKillAnimationEntity(kill.get());
                m_unspawnedEntities.push_back(EntitySpawn(std::move(kill)));
            }
            if(auto name = xml->Attribute("newName"))
                entity->setName(name);

            const tinyxml2::XMLElement* physic = nullptr;
            const tinyxml2::XMLElement* shape = nullptr;
            findPhysicAndShapeTag(physic, shape, xml, templates);
            if(auto draworder = it->second->FloatAttribute("draworder"))
                entity->setDrawOrder(draworder);
            if(physic != nullptr && shape != nullptr)
            {
                PhysicsParser parser(ProviderContext(entity, entity, entity, entity, m_cloneHandler));
                auto physics = parser.parse(*physic, *shape);
                physics.bodyDef.position = b2Vec2(static_cast<float>(utility::toMeter(position.x)),
                                                  static_cast<float>(utility::toMeter(position.y)));
                entity->bindDefs(physics.fixtureDef, physics.shapes, physics.bodyDef, &m_world);
                entity->bindRotationController(std::move(physics.controllers.rotation));
                entity->bindPositionController(std::move(*physics.controllers.position[0]), std::move(*physics.controllers.position[1]));
            }
            return std::move(original);
        }
        return nullptr;
    }

    return parseEntity(match->second, position, templates, bindInstantly);
}

void Level::findPhysicAndShapeTag(
    const tinyxml2::XMLElement*& physic,
    const tinyxml2::XMLElement*& shape,
    const tinyxml2::XMLElement* entity,
    Templates& templates)
{
    auto physics = entity->FirstChildElement("physics");
    if(physics != nullptr)
    {
        // Shape template exists
        if(auto shapeName = physics->Attribute("shape"))
        {
            auto shapeTemplate = templates.shapes.find(shapeName);
            if(shapeTemplate != end(templates.shapes))
                shape = shapeTemplate->second;
        }
        // Physics doesn't use a template
        else
            shape = physics->FirstChildElement("shape");

        if(auto physicsName = physics->Attribute("name"))
        {
            auto physicsTemplate = templates.physics.find(physicsName);
            if(physicsTemplate != end(templates.physics))
                physic = physicsTemplate->second;

            if(physic == nullptr)
                throw std::runtime_error(utility::replace(utility::translateKey("UnknownPhysicReference"), physicsName));
        }
        else
            physic = physics;
    }
}

std::unique_ptr<Entity> Level::parseEntity(
    const tinyxml2::XMLElement* entity,
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
    const tinyxml2::XMLElement* physic = nullptr;
    const tinyxml2::XMLElement* shape = nullptr;
    findPhysicAndShapeTag(physic, shape, entity, templates);
    
    std::unique_ptr<Entity> graphicalObject = createEntity(entity, position, shape, physic, templates, bindInstantly);
    if(entity->Attribute("z") != nullptr)
        graphicalObject.get()->setDrawOrder(entity->FloatAttribute("z"));

    return graphicalObject;
}

bool Level::validate(const tinyxml2::XMLDocument& document) const
{
    if(document.Error()) // Error while loading file
    {
        document.PrintError();
        return false;
    }
    return true;
}

std::unique_ptr<Entity> Level::createEntity(
    const tinyxml2::XMLElement* xml,
    sf::Vector2u& position,
    const tinyxml2::XMLElement* shape,
    const tinyxml2::XMLElement* physic,
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
            entity = std::unique_ptr<Teeter>(new Teeter(m_config.get<float>("MouseScale"), m_cloneHandler));
        else if(typeName == "ball")
        {
            isBullet = true;
            std::unique_ptr<Entity> spawn = parseEntityReference("onRespawn", xml, templates);
            if(spawn != nullptr)
                m_unspawnedEntities.push_back(EntitySpawn(std::move(spawn)));

            std::unique_ptr<Entity> kill = parseEntityReference("onKill", xml, templates);
            if(kill != nullptr)
                m_unspawnedEntities.push_back(EntitySpawn(std::move(kill)));

            float autoKillSpeed = xml->FloatAttribute("autoKillSpeed");
            auto ball = std::unique_ptr<Ball>(new Ball(m_config.get<float>("BallResetTime"), autoKillSpeed, m_cloneHandler, spawn.get(), kill.get()));

            auto context = ProviderContext(this, ball.get(), ball.get(), ball.get(), m_cloneHandler)
                                          .withFunctions(templates.functions);
            LevelFileLoader loader(context, m_resourceManager);
            ball->bindTrail(loader.parseTrail(*xml));
            entity = std::move(ball);
        }
        else if(typeName == "target")
        {
            entity = std::unique_ptr<Entity>(new Entity(Entity::Target, m_cloneHandler, respawnable, autoStop));
            m_normalTargetPoints = xml->IntAttribute("points");
            if(m_normalTargetPoints == 0)
                m_normalTargetPoints = 100;
            m_totalTarget++;
        }
        else if(typeName == "bonustarget")
        {
            entity = std::unique_ptr<Entity>(new Entity(Entity::BonusTarget, m_cloneHandler, respawnable, autoStop));
            m_bonusTargetPoints = xml->IntAttribute("points");
            if(m_bonusTargetPoints == 0)
                m_bonusTargetPoints = 10;
        }
        else if(typeName == "entityFactory")
        {
            float min = xml->FloatAttribute("minDelayTime");
            float max = xml->FloatAttribute("maxDelayTime");
            std::string productName = xml->Attribute("productName");
            b2Vec2 spawnOffset(xml->FloatAttribute("spawnOffsetX"), xml->FloatAttribute("spawnOffsetY"));

            std::unique_ptr<Entity> product = parseEntityFromTemplate(productName, templates, position, false);

            auto factory = std::unique_ptr<EntityFactory>(new EntityFactory(m_cloneHandler, respawnable, autoStop, 
                                                                            product, min, max, spawnOffset));

            factory->setOwnSpeedTransfer(xml->BoolAttribute("transferOwnSpeed"));

            factory->registerForDelivery([&](std::unique_ptr<Entity>& product){
                m_entitiesToSpawn.push_back(std::move(product));
            });
            entity = std::move(factory);
        }
        else // No type or unknown type specified => normal Entity
        {
            entity = std::unique_ptr<Entity>(new Entity(Entity::None, m_cloneHandler, respawnable, autoStop));
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

    auto context = ProviderContext(this, entity.get(), entity.get(), entity.get(), m_cloneHandler)
                                  .withFunctions(templates.functions);
    AnimationParser loader(context, m_resourceManager);

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
                if(auto animation = loader.parseSingle(*element))
                {
                    if(animation->getBufferId() == UINT_MAX)
                        animation->setBufferId(m_defaultTargetBuffer);
                    animation->setValueOf("cloneId", static_cast<float>(copy));
                    entity->bindAnimation(std::move(animation));
                }
            }
        }
    }
    if(physic == nullptr)
        entity->setPosition(b2Vec2(static_cast<float>(utility::toMeter(position.x)),
                                   static_cast<float>(utility::toMeter(position.y))));

    if(auto constants = xml->FirstChildElement("constants"))
        ValueParser::parseConstants(*constants, *entity);

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
            auto soundContext = ProviderContext(nullptr, nullptr, nullptr, nullptr, m_cloneHandler)
                                               .withFunctions(templates.functions);
            ProviderParser parser(context);
            std::unique_ptr<ValueProvider> provider(parser.parseSingle(*sound->FirstChildElement()));
            otherSounds.push_back(std::unique_ptr<SoundTrigger>(new SoundTrigger(soundName, m_resourceManager.getSoundManager(), std::move(provider))));
        }
        entity->bindOtherSounds(std::move(otherSounds));
    }

    if(physic != nullptr && shape != nullptr)
    {
        PhysicsParser parser(ProviderContext(entity.get(), entity.get(), entity.get(), entity.get(), m_cloneHandler));
        auto physics = parser.parse(*physic, *shape);
        physics.bodyDef.bullet = isBullet;
        physics.bodyDef.position = b2Vec2(static_cast<float>(utility::toMeter(position.x)),
                                          static_cast<float>(utility::toMeter(position.y)));
        entity->bindDefs(physics.fixtureDef, physics.shapes, physics.bodyDef, &m_world);
        entity->bindRotationController(std::move(physics.controllers.rotation));
        entity->bindPositionController(std::move(*physics.controllers.position[0]), std::move(*physics.controllers.position[1]));
    }

    if(auto collider = xml->FirstChildElement("onCollision"))
        parseCollider(entity.get(), collider, templates);

    if(entity->hasPhysics())
    {
        if(xml->Attribute("collideWithBall") != nullptr)
            entity->setCollideWithBall(xml->BoolAttribute("collideWithBall"));
        else
            entity->setCollideWithBall(true);

        if(xml->Attribute("collideWithOtherEntity") != nullptr)
            entity->setCollideWithOtherEntity(xml->BoolAttribute("collideWithOtherEntity"));
        else
            entity->setCollideWithOtherEntity(true);

        if(auto filter = xml->FirstChildElement("collides"))
            parseCollisionFilter(entity.get(), filter, templates);

        if(bindInstantly)
            entity->bindBody();
    }

    if(auto jointXml = xml->FirstChildElement("joints"))
    {
        auto parser = JointParser(*jointXml,
                                  m_resourceManager,
                                  *entity.get(),
                                  *entity.get(),
                                  m_cloneHandler,
                                  m_world,
                                  *entity->getBody(),
                                  m_defaultTargetBuffer);
        auto joints = parser.parse();
        for(auto it = begin(joints); it != end(joints); ++it)
        {
            for(auto ani = begin(it->animations); ani != end(it->animations); ++ani)
                entity->bindAnimation(std::move(*ani));
            entity->addJoint(std::move(it->joint));
        }
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
    const tinyxml2::XMLElement* xml,
    Templates& templates)
{

    for(auto child = xml->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        if(std::string(child->Name()) == "changeProperty")
        {
            std::unique_ptr<ChangePropertyCollisionHandler> collider(new ChangePropertyCollisionHandler(child->Attribute("name"), this));
            auto context = ProviderContext(collider.get(), nullptr, collider.get(), collider.get(), m_cloneHandler)
                                          .withFunctions(templates.functions);
            ProviderParser parser(context);
            std::unique_ptr<ValueProvider> provider(parser.parseSingle(*child->FirstChildElement()));
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

            auto pos = sf::Vector2u(0, 0);
            std::unique_ptr<Entity> spawned(parseEntityFromTemplate(name, templates, pos, false));
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

std::unique_ptr<CollisionHandler> Level::parseShowLabelHandler(const tinyxml2::XMLElement* xml)
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
            RandomProvider angleRandom(0.f, utility::toRadian(360.f));
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
    const tinyxml2::XMLElement* xml,
    Templates& templates)
{
    std::string name(xml->Name());
    if(name == "always")
        return std::unique_ptr<CollisionFilter>(new Always());
    else if(name == "never")
        return std::unique_ptr<CollisionFilter>(new Never());
    else if(name == "spawnEntity")
    {
        auto name = xml->Attribute("name");
        if(!name)
            throw std::runtime_error(utility::translateKey("SpawnWithoutName"));

        auto pos = sf::Vector2u(0, 0);
        std::unique_ptr<Entity> spawned(parseEntityFromTemplate(name, templates, pos, false));
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
    else if(name == "copySpawnLocation")
    {
        std::unique_ptr<CollisionFilter> subFilter = getCollisionFilter(entity, xml->FirstChildElement(), templates);
        std::unique_ptr<ChangeBallSpawnFilter> filter(new ChangeBallSpawnFilter(std::move(subFilter)));
        return std::move(filter);
    }
    else if(name == "setBallVelocity")
    {
        std::unique_ptr<CollisionFilter> subFilter = getCollisionFilter(entity, xml->FirstChildElement(), templates);
        std::unique_ptr<ChangeBallVelocityFilter> filter(new ChangeBallVelocityFilter(
            xml->FloatAttribute("x"),
            xml->FloatAttribute("y"),
            std::move(subFilter)));
        return std::move(filter);
    }
    else if(name == "applyForce")
    {
        b2Vec2 force(xml->FloatAttribute("x"), xml->FloatAttribute("y"));
        std::unique_ptr<CollisionFilter> subFilter = getCollisionFilter(entity, xml->FirstChildElement(), templates);
        std::unique_ptr<ApplyForceFilter> filter(new ApplyForceFilter(force, std::move(subFilter)));
        return std::move(filter);
    }
    else if(name == "changeGravity")
    {
        bool target = true;//std::string("entity") == child->Attribute("target");
        b2Vec2 gravity(xml->FloatAttribute("x"), xml->FloatAttribute("y"));
        std::unique_ptr<ChangeGravityFilter> filter(new ChangeGravityFilter(m_gravity, gravity, target, this));
        auto context = ProviderContext(filter.get(), nullptr, filter.get(), filter.get(), m_cloneHandler)
                                      .withFunctions(templates.functions);
        ProviderParser parser(context);
        std::unique_ptr<ValueProvider> provider(parser.parseSingle(*xml->FirstChildElement()));
        filter->bindProvider(std::move(provider));
        return std::move(filter);
    }
    else if(name == "propertyFilter")
    {
        bool target = std::string("entity") == xml->Attribute("target");
        std::unique_ptr<PropertyFilter> filter(new PropertyFilter(target, this));
        auto context = ProviderContext(filter.get(), nullptr, filter.get(), filter.get(), m_cloneHandler)
                                      .withFunctions(templates.functions);
        ProviderParser parser(context);
        std::unique_ptr<ValueProvider> provider(parser.parseSingle(*xml->FirstChildElement()));
        filter->bindProvider(std::move(provider));
        return std::move(filter);
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("UnknownFilter"), xml->Name()));
}

void Level::parseCollisionFilter(
    Entity* entity,
    const tinyxml2::XMLElement* xml,
    Templates& templates)
{
    entity->bindCollisionFilter(getCollisionFilter(entity, xml->FirstChildElement(), templates));
}

std::unique_ptr<Entity> Level::parseEntityReference(
    const std::string& key,
    const tinyxml2::XMLElement* xml,
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

    auto pos = sf::Vector2u(0, 0);
    std::unique_ptr<Entity> spawned(parseEntityFromTemplate(name, templates, pos, false));
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

void Level::parseGameplayAttributes(const tinyxml2::XMLElement* xml)
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
