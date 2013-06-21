#pragma once

#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "Ball.hpp"
#include "../rendering/DebugDraw.hpp"
#include "Background.hpp"
#include "../animation/TimedObject.hpp"
#include "../animation/provider/RandomProvider.hpp"
#include "./collision./ContactListener.hpp"
#include "../rendering/Drawable.hpp"
#include "../ScrollView.hpp"
#include "SoundManager.hpp"
#include "../gui/FPSCounter.hpp"
#include "../gui/LineLabel.hpp"

#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Collision/Shapes/b2Shape.h>

#include <memory> // unique_ptr
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>

class Config;

class ResourceManager;

namespace tinyxml2
{
    class XMLDocument;
    class XMLElement;
}

/// Manages the different levels and is
/// also resposible for loading them.
class Level :
    public TimedObject,
    public Drawable,
    private VariableHandler,
    private CollisionFilter,
    private CollisionHandler
{
public:
    /// Construct a level from the given level number
    Level(const unsigned int level, ResourceManager& resourceManager, Config& config);
    ~Level();

    virtual void restartAt(const float time);
    void update(const float dt, sf::RenderTarget& screen);
    void draw(const DrawParameter& param);
    void adjustView(sf::RenderTarget& screen);

    void adaptToMouse();

    /// Return the level number
    const unsigned int number();

    const float getWidth() const;
    const float getHeight() const;

    const int getTotalTarget() const;
    const int getRemainingTarget() const;

    const int getPoints() const;
    const int getRemainingBall() const;

    const bool isLevelPassed() const;
    const bool isLevelFailed() const;

    const float getRemainigTime() const;

    virtual float getValueOf(const std::string& name) const;
    virtual void setValueOf(const std::string& name, const float value);

    const sf::Vector2f getBallCoords() const;

    void setTimeAttackMode(bool timeAttackMode);

private:

    struct Templates
    {
    public:
        std::unordered_map<std::string, tinyxml2::XMLElement*> physics;
        std::unordered_map<std::string, tinyxml2::XMLElement*> shapes;
        std::unordered_map<std::string, tinyxml2::XMLElement*> entities;
        std::unordered_map<std::string, tinyxml2::XMLElement*> functions;
    };
    
    void createLabelAt(const Entity* target, const std::string& fontName, const int points);
    void createLabelAt(const Entity* target, const std::string& fontName, const std::string& text);
    void createLabelAt(const sf::Vector2f& position, const std::string& fontName, const std::string& text);
    void respawnDeadBalls();
    void trackBallMovement(float elapsedTime);
    void spawnPendingEntities(float currentTime);
    void cleanupKilledEntities();
    void updatePointLabels();

    virtual bool shouldCollide(Entity* entityA, Entity* entityB);
    virtual void onCollision(Entity* entityA, Entity* entityB, const b2Vec2& point, const float impulse);
    void killTarget(Entity* target);
    void killBonusTarget(Entity* target);
    void prepareEntityForSpawn(const b2Vec2& position, const Entity* spawn);

    void updateTimeAttackeMode(const float elapsedTime);

    /// Load the level after m_number
    bool load();

    bool validate(const tinyxml2::XMLDocument& document);
    
    void parseObjects(
        Templates& templates,
        tinyxml2::XMLElement* root,
        std::vector<std::unique_ptr<tinyxml2::XMLDocument>>& docs);

    void parseTemplates(
        Templates& templates,
        tinyxml2::XMLElement* root,
        std::vector<std::unique_ptr<tinyxml2::XMLDocument>>& docs);

    std::unique_ptr<Entity> createEntity(
        tinyxml2::XMLElement* xml,
        const sf::Vector2u& position,
        tinyxml2::XMLElement* shape,
        tinyxml2::XMLElement* physic,
        Templates& templates,
        bool bindInstantly = true);

    std::unique_ptr<Entity> parseEntityFromTemplate(
        std::string name,
        Templates& templates,
        const sf::Vector2u& position,
        bool bindInstantly = true);

    std::unique_ptr<Entity> parseEntity(
        tinyxml2::XMLElement* xml,
        const sf::Vector2u& position,
        Templates& templates,
        bool bindInstantly = true);

    void parseCollider(
        Entity* entity,
        tinyxml2::XMLElement* collider,
        Templates& templates);

    void parseCollisionFilter(
        Entity* entity,
        tinyxml2::XMLElement* xml,
        Templates& templates);

    std::unique_ptr<CollisionFilter> getCollisionFilter(
        Entity* entity,
        tinyxml2::XMLElement* xml,
        Templates& templates);

    std::unique_ptr<Entity> parseBallSpawnAnimation(
        tinyxml2::XMLElement* xml,
        Templates& templates);

    void parseGameplayAttributes(tinyxml2::XMLElement* xml);

    std::unique_ptr<CollisionHandler> parseShowLabelHandler(tinyxml2::XMLElement* xml);

    /// Construct the full level filename from the level number
    const std::string filename();
    const std::string pathname() const;

private:

    b2World m_world;
    b2Vec2 m_defaultGravity;

    ResourceManager& m_resourceManager;

    SoundManager m_soundManager;

    unsigned int m_number;

    std::map<std::string, float> m_variables;
    Entity* m_updatingEntity;
    std::vector<std::unique_ptr<Entity>> m_entities;
    std::vector<std::unique_ptr<Entity>> m_entitiesToSpawn;
    std::vector<std::unique_ptr<Entity>> m_unspawnedEntities;

    // HACK: This should be in the Entity class, but since unique_ptr sucks with VS10...
    std::vector<std::unique_ptr<b2Shape>> m_shapes;

    std::unique_ptr<Background> m_background;

    std::unique_ptr<ContactListener> m_contactListener;

    float m_timeStep;
    int m_velocityIterations;
    int m_positionIterations;

    float m_width;
    float m_height;

    float m_lastTime;

    Config& m_config;

    bool m_debugDraw;

    ScrollView m_scrollView;

    Ball* m_ball;
    int m_remainingBall;

    // For trackBallMovement().
    struct BallMovement
    {
        BallMovement() : time(0.0f), distance(0.0f) {}
        BallMovement(float time, float dist) : time(time), distance(dist) {}
        float time;
        float distance;
    };
    std::queue<BallMovement> m_ballTravelDistances;
    float m_ballImpulseTime;
    b2Vec2 m_lastBallPosition;
    float m_ballTravelDistance;
    RandomProvider m_ballImpulseAngle;

    int m_totalTarget;
    int m_remainingTarget;

    int m_points;
    int m_multiHit;

    float m_remainingTime;
    float m_totalTime;

    struct TimedLabel
    {
    public:

        TimedLabel(std::unique_ptr<LineLabel> label, const float time)
        {
            this->label = std::move(label);
            this->time = time;
        }

        std::unique_ptr<LineLabel> label;
        float time;
    };
    
    std::vector<std::unique_ptr<TimedLabel>> m_pointLabels;

    bool m_levelPass;

    bool m_timeAttackMode;

    struct TargetToRespawn
    {
    public:
        TargetToRespawn(Entity* target, const float time)
        {
            this->respawnAt = time;
            this->target = target;
        }

        float respawnAt;
        Entity* target;
    };

    std::vector<TargetToRespawn> m_unspawnedTarget;
};

#endif // LEVEL_HPP
