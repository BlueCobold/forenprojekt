
#include "ColliderParser.hpp"

#include "ProviderParser.hpp"
#include "../animation/provider/RandomProvider.hpp"
#include "../model/Entity.hpp"
#include "../model/collision/handler/ApplyImpulseCollisionHandler.hpp"
#include "../model/collision/handler/BallTeleportCollisionHandler.hpp"
#include "../model/collision/handler/ChangePropertyCollisionHandler.hpp"
#include "../model/collision/handler/SpawnEntityCollisionHandler.hpp"
#include "../model/collision/handler/GenericCollisionHandler.hpp"

enum SpawnLocation
{
    BallLoc = 1,
    OwnerLoc = 2,
    ContactPoint = 3
};

std::unique_ptr<CollisionHandler> ColliderParser::parse(const tinyxml2::XMLElement& xml)
{
    for(auto child = xml.FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        std::string type(child->Name());
        if(type == "changeProperty")
        {
            std::unique_ptr<ChangePropertyCollisionHandler> collider(new ChangePropertyCollisionHandler(child->Attribute("name"), &m_context.variableHandler));
            auto context = ProviderContext(collider.get(), nullptr, collider.get(), collider.get(), m_context.cloneHandler)
                                          .withFunctions(m_templates.functions);
            ProviderParser parser(context);
            collider->bindProvider(parser.parseSingle(*child->FirstChildElement()));
            return std::move(collider);
        }
        else if(type == "applyImpulse")
        {
            return std::unique_ptr<ApplyImpulseCollisionHandler>(new ApplyImpulseCollisionHandler(child->FloatAttribute("x"),
                                                                                                  child->FloatAttribute("y")));
        }
        else if(type == "ballTeleport")
        {
            return std::unique_ptr<BallTeleportCollisionHandler>(new BallTeleportCollisionHandler(child->FloatAttribute("x"),
                                                                                                  child->FloatAttribute("y"),
                                                                                                  !child->BoolAttribute("keepSpeed")));
        }
        else if(type == "bonusTime")
        {
            int bonusTime = child->IntAttribute("value");
            auto timeFunc = m_context.timeFunc;
            std::unique_ptr<GenericCollisionHandler> collider(new GenericCollisionHandler(
            [=](Entity& entityA, Entity& entityB, const b2Vec2& point, const float impulse)
            {
                timeFunc(bonusTime);
            }));
            return std::move(collider);
        }
        else if(type == "killBall")
        {
            auto isInvul = m_context.isInvul;
            auto getBall = m_context.ball;
            auto spawnFunc = m_context.spawnFunc;
            std::unique_ptr<GenericCollisionHandler> collider(new GenericCollisionHandler(
            [=](Entity& entityA, Entity& entityB, const b2Vec2& point, const float impulse)
            {
                if(isInvul())
                    return;
                if(entityA.getType() != Entity::Ball && entityB.getType() != Entity::Ball)
                    throw std::runtime_error(utility::replace(utility::translateKey("EntityNoCollision"), "Ball"));
                auto& ball = getBall();
                ball.blowUp();
                if(auto killAnimation = ball.getKillAnimationEntity())
                    spawnFunc(ball.getPosition(), *killAnimation);
            }));
            return std::move(collider);
        }
        else if(type == "showLabel")
        {
            return parseShowLabelHandler(*child);
        }
        else if(type == "spawnEntity")
        {
            auto name = child->Attribute("name");
            if(!name)
                throw std::runtime_error(utility::translateKey("SpawnWithoutName"));

            auto spawned = m_parser(name);
            if(spawned == nullptr)
                throw std::runtime_error(utility::replace(utility::translateKey("UnknownEntityName"), name));

            SpawnLocation location = BallLoc;
            auto loc = child->Attribute("location");
            if(loc && std::string("contact") == loc)
                location = ContactPoint;
            else if(loc && std::string("entity") == loc)
                location = OwnerLoc;

            auto spawnFunc = m_context.spawnFunc;
            std::unique_ptr<SpawnEntityCollisionHandler> handler(new SpawnEntityCollisionHandler(
            [=](const Entity& owner, const Entity& spawned, const b2Vec2& contactPoint)
            {
                switch(location)
                {
                    case BallLoc:
                        spawnFunc(m_context.ball().getPosition(), spawned);
                        break;
                    case OwnerLoc:
                        spawnFunc(owner.getPosition(), spawned);
                        break;
                    case ContactPoint:
                        spawnFunc(contactPoint, spawned);
                        break;
                }
            }, m_target, *spawned));
            return std::move(handler);
        }
        else
            throw std::runtime_error(utility::replace(utility::translateKey("UnknownCollider"), child->Name()));
    }
    return nullptr;
}

std::unique_ptr<CollisionHandler> ColliderParser::parseShowLabelHandler(const tinyxml2::XMLElement& xml)
{
    std::string font(xml.Attribute("font"));
    auto distance = xml.FloatAttribute("offset");
    
    std::vector<std::string> labels;
    for(auto child = xml.FirstChildElement("label"); child != nullptr; child = child->NextSiblingElement("label"))
        labels.push_back(utility::translateKey(child->GetText()));

    float time = -1;
    auto source = m_context.timeSource;
    auto labelFunc = m_context.labelFunc;
    return std::unique_ptr<GenericCollisionHandler>(
        new GenericCollisionHandler([=](Entity& entityA, Entity& entityB, const b2Vec2& point, const float impulse)
        mutable {
            auto now = source.getPassedTime();
            // prevent a spam of labels... only one per second per entity!
            if(time > now)
                return;

            time = now + 1;
            RandomProvider lengthRandom(0.f, distance);
            RandomProvider angleRandom(0.f, utility::toRadian(360.f));
            RandomProvider textIndex(0.f, static_cast<float>(labels.size()));

            std::string text = labels[static_cast<int>(textIndex.getValue())];
            auto angle = angleRandom.getValue();
            auto length = lengthRandom.getValue();

            auto pos = sf::Vector2f(
                utility::toPixel(entityB.getPosition().x), 
                utility::toPixel(entityB.getPosition().y));
            sf::Vector2f offset(length * sinf(angle), length * cosf(angle));

            labelFunc(offset + pos, font, text);
        }));
}
