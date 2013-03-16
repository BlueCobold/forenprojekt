#include "Level.hpp"
#include "Entity.hpp"
#include "../rendering/DrawParameter.hpp"
#include "../resources/Config.hpp"
#include "../resources/ResourceManager.hpp"
#include "../Utility.hpp" // toString, toMeter
#include "../Input.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/System/Err.hpp>

#include <Box2D/Common/b2Draw.h>

#include <algorithm>
#include <ostream> // endl
#include <string>
#include <unordered_map>
#include <utility> // pair, make_pair, move

Level::Level(const unsigned int level, ResourceManager& resourceManager, Config& config) :
    m_number(level),
    m_resourceManager(resourceManager),
    m_world(b2Vec2(0.f, 9.81f)),
    m_timeStep(1.f / 60.f),
    m_velocityIterations(4),
    m_positionIterations(4),
    m_config(config),
    m_soundManager(resourceManager),
    m_totalTarget(0),
    m_remainingTarget(0),
    m_points(0),
    m_multiHit(0),
    m_ball(nullptr),
    m_updatingEntity(nullptr)
{
    m_world.SetAllowSleeping(false);
    m_debugDraw = false;
    m_contactListener.reset(new ContactListener(this, this));
    load();
}

Level::~Level()
{

}

void Level::restartAt(const float time)
{
    if(m_background != nullptr)
        m_background->restartAt(time);
    TimedObject::restartAt(time);
    for(auto it = begin(m_entities); it != end(m_entities); ++it)
        (*it)->restartAt(time);
    m_lastTime = time;
}

void Level::update(const float elapsedTime, sf::RenderTarget& screen)
{
    m_soundManager.update();

    m_timeStep = elapsedTime - m_lastTime;
    m_velocityIterations = std::max(1, 4);
    m_positionIterations = m_velocityIterations;

    int steps = std::min(20, std::max(1, static_cast<int>(ceilf(m_timeStep / (1 / (60.0f * 2))))));
    float delta = 0;
    for(int i=1; i<=steps; i++)
    {
        utility::Mouse.interpolate(steps, i);

        delta = (i * m_timeStep) / steps;

        TimedObject::updateCurrentTime(m_lastTime + delta);

        auto it = begin(m_entities);
        while(it != end(m_entities))
        {
            if((*it)->killed())
            {
                (*it)->unbindBody();
                it = m_entities.erase(it);
            }
            else
                ++it;
        }
        for(auto it = std::begin(m_entitiesToSpawn); it != std::end(m_entitiesToSpawn); ++it)
        {
            auto e = std::move(*it);
            e->restartAt(m_lastTime + delta);
            e->bindBody();
            m_entities.push_back(std::move(e));
        }
        m_entitiesToSpawn.clear();

        m_world.Step(m_timeStep / steps, m_velocityIterations, m_positionIterations);

        for(auto it = begin(m_entities); it != end(m_entities); ++it)
        {
            m_updatingEntity = (*it).get();
            if((*it)->getType() == Entity::Ball)
            {
                if(m_ball->getBallLost())
                {
                    m_points -= 10;
                    m_multiHit = 0;
                    m_world.SetGravity(m_defaultGravity);
                }
            }
            (*it)->update(m_lastTime + delta);
        }
        m_updatingEntity = nullptr;
    }

    if(utility::Keyboard.isKeyDown(sf::Keyboard::D))
        m_debugDraw = !m_debugDraw;

    m_lastTime = elapsedTime;

    sf::Vector2f ballpos = sf::Vector2f(utility::toPixel(m_ball->getPosition().x), utility::toPixel(m_ball->getPosition().y));
    m_scrollView.setZoomFactor(1.0f);
    m_scrollView.adjustView(ballpos, screen);

    if(m_background != nullptr)
        m_background->update(elapsedTime, screen.getView());

#ifdef _DEBUG
    if(utility::Keyboard.isKeyPressed(sf::Keyboard::Z))
    {
        m_scrollView.setZoomFactor(3.0f);
        m_scrollView.adjustView(ballpos, screen);
    }
#endif
}

float Level::getValueOf(const std::string& name) const
{
    auto match = m_variables.find(name);
    if(match == end(m_variables))
    {
        if(m_updatingEntity == nullptr)
            throw std::runtime_error(utility::replace(utility::translateKey("GetVariable"), name));
        return m_updatingEntity->getValueOf(name);
    }
    return match->second;
}

void Level::setValueOf(const std::string& name, const float value)
{
    if(m_updatingEntity == nullptr)
        m_variables[name] = value;
    else
    {
        auto match = m_variables.find(name);
        if(match == end(m_variables))
            m_updatingEntity->setValueOf(name, value);
        else
            m_variables[name] = value;
    }
}

bool Level::shouldCollide(Entity* entityA, Entity* entityB)
{
    if(entityB->getType() == Entity::Ball)
    {
        if(!entityA->shouldCollide(entityB))
            return false;

        if(entityA->getType() == Entity::Target)
            killTarget(entityA);
        else if(entityA->getType() == Entity::Teeter)
            m_multiHit = 0;

        return entityA->doesCollideWithBall();
    }
    else if(entityA->getType() == Entity::Ball)
    {
        if(!entityB->shouldCollide(entityA))
            return false;

        if(entityB->getType() == Entity::Target)
            killTarget(entityB);
        else if(entityB->getType() == Entity::Teeter)
            m_multiHit = 0;

        return entityB->doesCollideWithBall();
    }

    return true;
}

void Level::killTarget(Entity* target)
{
    target->kill();
    m_remainingTarget--;
    m_points += 100 + m_multiHit * 50;
    m_multiHit++;
}

void Level::onCollision(Entity* entityA, Entity* entityB)
{
    if(entityA->getType() == Entity::Ball)
    {
        float velocityA = abs(entityA->getBody()->GetLinearVelocity().x) + abs(entityA->getBody()->GetLinearVelocity().y);
        if(entityB->getSoundName().length() > 0)
            entityB->getSoundManager()->play(entityB->getSoundName(), velocityA);
        entityB->onCollide(entityA);
    }
    else if(entityB->getType() == Entity::Ball)
    {
        float velocityB = abs(entityB->getBody()->GetLinearVelocity().x) + abs(entityB->getBody()->GetLinearVelocity().y);
        if(entityA->getSoundName().length() > 0)
            entityA->getSoundManager()->play(entityA->getSoundName(), velocityB);
        entityA->onCollide(entityB);
    }
}

void Level::draw(const DrawParameter& param)
{
    if(m_background != nullptr)
        m_background->draw(param);

    for(auto it = begin(m_entities); it != end(m_entities); ++it)
        it->get()->draw(param);

    if(m_debugDraw)
    {
        DebugDraw d(param.getTarget());
        d.SetFlags(b2Draw::e_shapeBit | b2Draw::e_centerOfMassBit);
        m_world.SetDebugDraw(&d);
        m_world.DrawDebugData();
    }
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

const float Level::getWidth() const
{
    return m_width;
}

const float Level::getHeight() const
{
    return m_height;
}

const int Level::getTotalTarget() const
{
    return m_totalTarget;
}

const int Level::getRemainingTarget() const
{
    return m_remainingTarget;
}

const int Level::getPoints() const
{
    return m_points;
}
const sf::Vector2f Level::getBallCoords() const
{
    return sf::Vector2f(utility::toPixel(m_ball->getPosition().x), 
                        utility::toPixel(m_ball->getPosition().y));
}