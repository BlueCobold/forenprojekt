#include "Level.hpp"
#include "Entity.hpp"
#include "Teeter.hpp"
#include "../rendering/DrawParameter.hpp"
#include "../resources/AppConfig.hpp"
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

Level::Level(const unsigned int level, ResourceManager& resourceManager, AppConfig& config) :
    m_number(level),
    m_resourceManager(resourceManager),
    m_world(b2Vec2(0.f, 9.81f)),
    m_timeStep(1.f / 60.f),
    m_velocityIterations(4),
    m_positionIterations(4),
    m_gravityFactor(1),
    m_config(config),
    m_totalTarget(0),
    m_remainingTarget(0),
    m_points(0),
    m_bonusTargetPoints(10),
    m_normalTargetPoints(100),
    m_multiHit(0),
    m_ball(nullptr),
    m_lostBallCounter(0),
    m_updatingEntity(nullptr),
    m_remainingBall(-1),
    m_levelPass(false),
    m_remainingTime(-1),
    m_totalTime(-1),
    m_levelEndingTime(0),
    m_timeAttackMode(false),
    m_ballTravelDistance(0.0f),
    m_ballImpulseTime(0.0f),
    m_ballImpulseAngle(-20.0f, 20.0f),
    m_goldMedal(0),
    m_silverMedal(0),
    m_bronzeMedal(0),
    m_levelName(""),
    m_lastTime(0),
    m_gravityGoody(sf::Keyboard::Num1, 2.f, m_gravity, config.get<int>("goodygravity")),
    m_invulnerableGoody(sf::Keyboard::Num2, 3.f, m_ball, config.get<int>("goodyinvulnerable")),
    m_extraBallGoody(sf::Keyboard::Num3, Goody::ExtraBallGoody, 0, 0, config.get<int>("goodyextraball")),
    m_extraTimeGoody(sf::Keyboard::Num4, Goody::ExtraTimeGoody, 0, 0, config.get<int>("goodyextratime")),
    m_currentSeletedGoody(0),
    m_playing(false)
{
    auto func = [this](Goody& sender)
    {
        this->onGoodyActivated(sender);
    };
    m_gravityGoody.registerForActivation(func);
    m_invulnerableGoody.registerForActivation(func);
    m_extraBallGoody.registerForActivation(func);
    m_extraTimeGoody.registerForActivation(func);

    m_world.SetAllowSleeping(false);
#ifdef _DEBUG
    m_debugDraw = false;
#endif
    m_contactListener = ContactListener(this, this);
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
    m_timeStep = elapsedTime - m_lastTime;

    if(m_totalTime > 0)
        m_remainingTime -= m_timeStep;

    m_velocityIterations = std::max(1, 4);
    m_positionIterations = m_velocityIterations;

    updateGoodyChoice();
    m_gravityGoody.update(elapsedTime);
    m_invulnerableGoody.update(elapsedTime);
    m_extraBallGoody.update(elapsedTime);
    m_extraTimeGoody.update(elapsedTime);
    if(m_playing)
        updateGoodyCharges();

    m_ball->setInvulnerable(m_invulnerableGoody.isActive());

    if(!m_levelPass)
        m_levelEndingTime = elapsedTime + 1.0f;

    int steps = std::min(20, std::max(1, static_cast<int>(ceilf(m_timeStep / (1 / (60.0f * 2))))));
    float delta = 0;
    for(int i=1; i<=steps; i++)
    {
        utility::Mouse.interpolate(steps, i);

        delta = (i * m_timeStep) / steps;

        TimedObject::updateCurrentTime(m_lastTime + delta);

        m_world.SetGravity(m_gravityGoody.getGravity());

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

    if(m_extraBallGoody.isActive() && m_remainingBall > 0)
        m_remainingBall++;
    if(m_extraTimeGoody.isActive() && m_totalTime > 0)
        m_remainingTime += 30.f;

    if(m_timeAttackMode)
        handleAutoRespawn();

#ifdef _DEBUG
    if(utility::Keyboard.isKeyDown(sf::Keyboard::D))
        m_debugDraw = !m_debugDraw;
#endif

    if(utility::Keyboard.isKeyDown(sf::Keyboard::Space) && utility::toPixel(m_ballTravelDistance) < 60 && m_ballImpulseTime < elapsedTime - 1.0f)
    {
        m_ballImpulseTime = elapsedTime;
        float angle = m_ballImpulseAngle.getValue();
        float g = m_world.GetGravity().Length();
        float strength = m_ball->getBody()->GetMass() * sqrt(2 * g * utility::toMeter<float>(100.0f));
        m_ball->getBody()->ApplyLinearImpulse(utility::rotate(-strength / g * m_world.GetGravity(), utility::toRadian<float, float>(angle)), m_ball->getBody()->GetWorldCenter(), false);
    }

    trackBallMovement(elapsedTime);

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
        if((*it)->getType() != Entity::Ball || !m_ball->getBallLost())
            continue;
        if(!m_invulnerableGoody.isActive())
        {
            m_points -= 10;
            m_multiHit = 0;
            m_gravity = m_defaultGravity;
            m_remainingBall -= 1;
            m_lostBallCounter++;
            if(!(m_remainingBall < 1 && m_remainingBall > -1))
                createLabelAt(m_ball, "red", -10);
        }

        const Ball* ball = dynamic_cast<const Ball*>((*it).get());
        if(ball->getSpawnAnimationEntity() != nullptr)
            prepareEntityForSpawn(ball->getPosition(), ball->getSpawnAnimationEntity());
    }
}

void Level::trackBallMovement(float elapsedTime)
{
    b2Vec2 ballPos = m_ball->getPosition();
    float distance = (ballPos - m_lastBallPosition).Length();
    m_ballTravelDistances.push(BallMovement(elapsedTime, distance));
    m_ballTravelDistance += distance;
    for(float t = m_ballTravelDistances.front().time; t < elapsedTime - 1.5;)
    {
        auto f = m_ballTravelDistances.front();
        m_ballTravelDistance -= f.distance;
        m_ballTravelDistances.pop();
        t = m_ballTravelDistances.front().time;
    }
    m_lastBallPosition = ballPos;
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
        auto entity = it->get();
        if(entity->killed())
        {
            entity->unbindBody();
            if(entity->isRespawnable())
            {
                if(entity->getType() == Entity::Target || entity->getType() == Entity::BonusTarget)
                    m_unspawnedEntities.push_back(EntitySpawn(std::move(*it), getCurrentTime() + 5.f));
                else
                    m_unspawnedEntities.push_back(EntitySpawn(std::move(*it)));
            }
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
        else if(entityB->getType() == Entity::BonusTarget)
            killBonusTarget(entityB);

        return entityB->doesCollideWithBall();
    }

    return true;
}

void Level::killTarget(Entity* target)
{
    target->kill();
    m_remainingTarget--;

    if(!m_timeAttackMode && m_remainingTarget < 1)
        m_levelPass = true;

    int earned = m_normalTargetPoints + m_multiHit * 50 + m_gravityGoody.getBonusPoints();
    m_points += earned;
    m_multiHit++;
    createLabelAt(target, "green", earned);
}
void Level::killBonusTarget(Entity* target)
{
    target->kill();
    int earned = m_bonusTargetPoints + m_multiHit * 50 + m_gravityGoody.getBonusPoints();
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
            position, sf::Vector2f(),
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

#ifdef _DEBUG
    if(m_debugDraw)
    {
        DebugDraw d(param.getTarget());
        d.SetFlags(b2Draw::e_shapeBit | b2Draw::e_centerOfMassBit);
        m_world.SetDebugDraw(&d);
        m_world.DrawDebugData();
    }
#endif
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
    if(m_levelEndingTime < m_lastTime)
        return m_levelPass;
    else
        return false;
}

const float Level::getRemainigTime() const
{
    return m_remainingTime;
}

const Goody Level::getGoody (const Goody::Type& type) const
{
    if(type == Goody::InvulnerableGoody)
        return m_invulnerableGoody;
    else if(type == Goody::GravityGoody)
        return m_gravityGoody;
    else if(type == Goody::ExtraTimeGoody)
        return m_extraTimeGoody;
    else if(type == Goody::ExtraBallGoody)
        return m_extraBallGoody;
    throw new std::runtime_error(utility::translateKey("InvalidGoody"));
}

void Level::onGoodyActivated(Goody &sender)
{
    m_invulnerableGoody.setSelected(false);
    m_gravityGoody.setSelected(false);
    m_extraTimeGoody.setSelected(false);
    m_extraBallGoody.setSelected(false);

    m_currentSeletedGoody = sender.getType();
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

void Level::handleAutoRespawn()
{
    auto it = begin(m_unspawnedEntities);
    while(it != end(m_unspawnedEntities))
    {
        if(it->respawnAt > 0 && it->respawnAt < getCurrentTime())
        {
            m_entitiesToSpawn.push_back(std::move(it->target));
            it = m_unspawnedEntities.erase(it);
        }
        else
            ++it;
    }
}
const Level::Medals Level::getMedal() const
{
    if(m_points >= m_goldMedal)
        return Level::Gold;
    else if(m_points >= m_silverMedal)
        return Level::Silver;
    else if(m_points >= m_bronzeMedal)
        return Level::Bronze;
    else
        return Level::none;
}

const std::string Level::getLevelName() const
{
    return m_levelName;
}

const int Level::getLostBalls() const
{
    return m_lostBallCounter;
}

const int Level::getMedal(const Level::Medals medal) const
{
    if(medal == Bronze)
        return m_bronzeMedal;
    else if(medal == Gold)
        return m_goldMedal;
    else if(medal == Silver)
        return m_silverMedal;
    else
        return 0;
}

void Level::updateGoodyChoice()
{
    if(utility::Mouse.isWheelMovedDown())
    {
        m_currentSeletedGoody++;
        if(m_currentSeletedGoody == Goody::Counter)
            m_currentSeletedGoody = 0;
    }
    if(utility::Mouse.isWheelMovedUp())
    {
        m_currentSeletedGoody--;
        if(m_currentSeletedGoody < 0)
            m_currentSeletedGoody = Goody::Counter - 1;
    }

    m_gravityGoody.setSelected(m_gravityGoody.getType() == m_currentSeletedGoody);
    m_invulnerableGoody.setSelected(m_invulnerableGoody.getType() == m_currentSeletedGoody);
    m_extraBallGoody.setSelected(m_extraBallGoody.getType() == m_currentSeletedGoody);
    m_extraTimeGoody.setSelected(m_extraTimeGoody.getType() == m_currentSeletedGoody);
}
void Level::updateGoodyCharges()
{
    m_config.set<int>("goodygravity", m_gravityGoody.getCharges());
    m_config.set<int>("goodyinvulnerable", m_invulnerableGoody.getCharges());
    m_config.set<int>("goodyextraball", m_extraBallGoody.getCharges());
    m_config.set<int>("goodyextratime", m_extraTimeGoody.getCharges());
}
void Level::onEnter()
{
    m_playing = true;

    m_gravityGoody.setCharges(m_config.get<int>("goodygravity"));
    m_invulnerableGoody.setCharges(m_config.get<int>("goodyinvulnerable"));
    m_extraBallGoody.setCharges(m_config.get<int>("goodyextraball"));
    m_extraTimeGoody.setCharges(m_config.get<int>("goodyextratime"));
}