#include "Level.hpp"
#include "Entity.hpp"
#include "Teeter.hpp"
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
    m_updatingEntity(nullptr),
    m_remainingBall(-1),
    m_levelPass(false),
    m_remainingTime(-1),
    m_totalTime(-1),
    m_timeAttackMode(false)
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

    if(m_totalTime > 0)
        m_remainingTime -= m_timeStep;

    m_velocityIterations = std::max(1, 4);
    m_positionIterations = m_velocityIterations;

    int steps = std::min(20, std::max(1, static_cast<int>(ceilf(m_timeStep / (1 / (60.0f * 2))))));
    float delta = 0;
    for(int i=1; i<=steps; i++)
    {
        utility::Mouse.interpolate(steps, i);

        delta = (i * m_timeStep) / steps;

        TimedObject::updateCurrentTime(m_lastTime + delta);

        cleanupKilledEntities();
        respawnDeadBalls();
        spawnPendingEntities(m_lastTime + delta);

        m_world.Step(m_timeStep / steps, m_velocityIterations, m_positionIterations);

        for(auto it = begin(m_entities); it != end(m_entities); ++it)
        {
            m_updatingEntity = (*it).get();
            m_updatingEntity->update(m_lastTime + delta);
        }
        m_updatingEntity = nullptr;

        updatePointLabels();
    }

    if(m_timeAttackMode)
        updateTimeAttackeMode(elapsedTime);

    if(utility::Keyboard.isKeyDown(sf::Keyboard::D))
        m_debugDraw = !m_debugDraw;

    m_lastTime = elapsedTime;

    if(m_background != nullptr)
        m_background->update(elapsedTime);

    adjustView(screen);

#ifdef _DEBUG
    if(utility::Keyboard.isKeyPressed(sf::Keyboard::Z))
    {
        sf::Vector2f ballpos = sf::Vector2f(utility::toPixel(m_ball->getPosition().x), utility::toPixel(m_ball->getPosition().y));
        m_scrollView.setZoomFactor(3.0f);
        m_scrollView.adjustView(ballpos, screen);
    }
#endif
}

void Level::adjustView(sf::RenderTarget& screen)
{
    sf::Vector2f ballpos = sf::Vector2f(utility::toPixel(m_ball->getPosition().x), utility::toPixel(m_ball->getPosition().y));
    m_scrollView.setZoomFactor(1.0f);
    m_scrollView.adjustView(ballpos, screen);

    if(m_background != nullptr)
        m_background->updateParallax(screen.getView());
}

void Level::respawnDeadBalls()
{
    for(auto it = begin(m_entities); it != end(m_entities); ++it)
    {
        m_updatingEntity = (*it).get();
        if((*it)->getType() != Entity::Ball || !m_ball->getBallLost())
            continue;

        m_points -= 10;
        m_multiHit = 0;
        m_world.SetGravity(m_defaultGravity);
        m_remainingBall -= 1;
        createLabelAt(m_ball, "red", -10);

        const Ball* ball = dynamic_cast<const Ball*>((*it).get());
        if(ball->getSpawnAnimationEntity() != nullptr)
            prepareEntityForSpawn(ball->getPosition(), ball->getSpawnAnimationEntity());
    }
}

void Level::spawnPendingEntities(float currentTime)
{
    for(auto it = std::begin(m_entitiesToSpawn); it != std::end(m_entitiesToSpawn); ++it)
    {
        auto e = std::move(*it);
        e->restartAt(currentTime);
        e->bindBody();
        m_entities.push_back(std::move(e));
    }
    m_entitiesToSpawn.clear();
}

void Level::cleanupKilledEntities()
{
    auto it = begin(m_entities);
    while(it != end(m_entities))
    {
        if((*it)->killed())
        {
            (*it)->unbindBody();
            if((*it)->isRespawnable())
                m_unspawnedEntities.push_back(std::move(*it));
            it = m_entities.erase(it);
        }
        else
            ++it;
    }
}

void Level::updatePointLabels()
{
    auto lit = begin(m_pointLabels);
    while(lit != end(m_pointLabels))
    {
        auto timedLabel = (*lit).get();
        timedLabel->label->updateProgress(getPassedTime());
        if(getPassedTime() - timedLabel->time > 1)
        {
            if(timedLabel->label->allProgressesFinished())
            {
                lit = m_pointLabels.erase(lit);
                continue;
            }
            ++lit;
            if(timedLabel->label->anyProgressRunning())
                continue;
            timedLabel->label->attachPositionProgress(
                Interpolation(0, 0, 1, getPassedTime()),
                Interpolation(0, -50, 1, getPassedTime()));
            timedLabel->label->attachAlphaProgress(
                Interpolation(1, 0, 1, getPassedTime()));
            timedLabel->label->updateProgress(getPassedTime());
        }
        else
            ++lit;
    }
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
        else if(entityA->getType() == Entity::BonusTarget)
            killBonusTarget(entityA);

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
        else if(entityA->getType() == Entity::BonusTarget)
            killBonusTarget(entityA);

        return entityB->doesCollideWithBall();
    }

    return true;
}

void Level::killTarget(Entity* target)
{
    if(target->isRespawnable() && m_timeAttackMode)
    {
        for(auto it = m_unspawnedTarget.begin(); it != m_unspawnedTarget.end(); ++it)
            if(it->target == target)
                return;

        TargetToRespawn targetToRespawn(target, Level::TimedObject::getCurrentTime() + 5.f);
        m_unspawnedTarget.push_back(targetToRespawn);
    }
    else
    {
        target->kill();
        if(m_remainingTarget < 1)
            m_levelPass = true;
    }

    int earned = 100 + m_multiHit * 50;
    m_points += earned;
    m_multiHit++;
    m_remainingTarget--;
    createLabelAt(target, "green", earned);
}
void Level::killBonusTarget(Entity* target)
{
    target->kill();
    int earned = 10 + m_multiHit * 50;
    m_points += earned;
    m_multiHit++;
    createLabelAt(target, "green", earned);
}
void Level::createLabelAt(const Entity* target, const std::string& fontName, const int number)
{
    std::string prefix;
    if(number > 0)
        prefix = std::string("+");
    createLabelAt(target, fontName, prefix + utility::toString(number));
}

void Level::createLabelAt(const Entity* target, const std::string& fontName, const std::string& text)
{
    return createLabelAt(sf::Vector2f(
                utility::toPixel(target->getPosition().x), 
                utility::toPixel(target->getPosition().y)),
                fontName,
                text);
}

void Level::createLabelAt(const sf::Vector2f& position, const std::string& fontName, const std::string& text)
{

    std::unique_ptr<LineLabel> label(new LineLabel(
            text,
            position,
            0,
            m_resourceManager.getBitmapFont(fontName),
            LineLabel::Centered));

    m_pointLabels.push_back(std::unique_ptr<TimedLabel>(new TimedLabel(std::move(label), getPassedTime())));
}

void Level::onCollision(Entity* entityA, Entity* entityB, const b2Vec2& point, const float impulse)
{
    if(entityA->getType() == Entity::Ball)
        entityB->onCollide(entityA, point, impulse);
    else if(entityB->getType() == Entity::Ball)
        entityA->onCollide(entityB, point, impulse);
}

void Level::draw(const DrawParameter& param)
{
    if(m_background != nullptr)
        m_background->draw(param);

    for(auto it = begin(m_entities); it != end(m_entities); ++it)
        (*it)->draw(param);

    for(auto it = begin(m_pointLabels); it != end(m_pointLabels); ++it)
        (*it)->label->draw(param);

    if(m_debugDraw)
    {
        DebugDraw d(param.getTarget());
        d.SetFlags(b2Draw::e_shapeBit | b2Draw::e_centerOfMassBit);
        m_world.SetDebugDraw(&d);
        m_world.DrawDebugData();
    }
}

void Level::adaptToMouse()
{
    for(auto it = m_entities.begin(); it != m_entities.end(); it++)
        if((*it)->getType() == Entity::Teeter)
            dynamic_cast<Teeter*>((*it).get())->adaptToMouse();
}

const unsigned int Level::number()
{
    return m_number;
}

const std::string Level::pathname() const
{
    return std::string("res/level/");
}

const std::string Level::filename()
{
    std::string filename = pathname();
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

const int Level::getRemainingBall() const
{
    return m_remainingBall;
}

const bool Level::isLevelPassed() const
{
    return m_levelPass;
}

const float Level::getRemainigTime() const
{
    return m_remainingTime;
}

const bool Level::isLevelFailed() const
{
    bool value = m_remainingBall < 1 && m_remainingBall > -1;
    // don't use the same variable to check - it can cause negative times
    // I had that quite often when debugging or lagging (dragging the window)
    value |= m_remainingTime < 0 && m_totalTime > -1.f;
    return value;
}
void Level::setTimeAttackMode(bool timeAttackMode)
{
    m_timeAttackMode = timeAttackMode;

    if(m_timeAttackMode)
    {
        m_remainingTime = 120.f;
        m_totalTime = 120.f;
    }
}

void Level::updateTimeAttackeMode(const float elapsedTime)
{
    auto it = begin(m_unspawnedTarget);
    while(it != end(m_unspawnedTarget))
    {
        if(!it->target->hidden() && !it->target->frozen())
        {
            it->target->hide();
            it->target->freeze();
        }
        if(it->respawnAt < elapsedTime)
        {
            it->target->unfreeze();
            it->target->unhide();
            m_remainingTarget++;
            it = m_unspawnedTarget.erase(it);
        }
        else
            ++it;
    }
}