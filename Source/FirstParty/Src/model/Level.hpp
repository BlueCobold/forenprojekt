#pragma once

#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "Ball.hpp"
#include "Background.hpp"
#include "EntitySpawn.hpp"
#include "GravityGoody.hpp"
#include "InvulnerableGoody.hpp"
#include "SoundManager.hpp"
#include "collision/ContactListener.hpp"
#include "../animation/TimedObject.hpp"
#include "../animation/CloneHandler.hpp"
#include "../animation/provider/RandomProvider.hpp"
#include "../gui/FPSCounter.hpp"
#include "../gui/LineLabel.hpp"
#include "../rendering/DebugDraw.hpp"
#include "../rendering/Drawable.hpp"
#include "../rendering/Sprite.hpp"
#include "../resources/GameEventRecorder.hpp"
#include "../resources/TemplateParser.hpp"
#include "../ScrollView.hpp"

#include <Box2D/Collision/Shapes/b2Shape.h>
#include <Box2D/Dynamics/b2World.h>

#include <memory> // unique_ptr
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <functional>

class AppConfig;

class ResourceManager;
class EntityFactory;

namespace tinyxml2
{
    class XMLDocument;
    class XMLElement;
}

struct LevelInfo
{
    std::string name;
    Sprite preview;
    int maxBalls;
    float time;

    LevelInfo() : maxBalls(0), time(0)
    { }
};

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
    Level(const std::string& file, const unsigned int level, std::unique_ptr<ResourceManager> resourceManager, AppConfig& config, bool loadInfoOnly = false);
    std::string getFileName();
    ~Level();

    void update(const double elapsedTime);
    void adjustView(const DrawParameter& params);

    void adaptToMouse();

    /// Return the level number
    unsigned int number() const;

    float getWidth() const;
    float getHeight() const;

    int getTotalTarget() const;
    int getRemainingTarget() const;

    int getPoints() const;
    int getRemainingBall() const;
    void addBall();

    bool isLevelPassed() const;
    bool isLevelFailed() const;

    double getRemainigTime() const;
    double getTotalTime() const;

    const Goody& getGoody(const Goody::Type& type) const;
    void onGoodyActivated(Goody& goody);

    sf::Vector2f getBallCoords() const;
    int getLostBalls() const;

    void setTimeAttackMode(bool timeAttackMode);
    bool isTimeAttackMode() const;

    enum Medals {Gold, Silver, Bronze, none};
    Level::Medals getMedal() const;
    int getMedal(const Level::Medals medal) const;
    std::string getLevelName() const;

    void onEnter();

    const std::list<GameEvent>& getGameEvents() const;

    double getLevelPlayTime() const;

    LevelInfo getInfo() const
    {
        return m_info;
    }

private:
#ifdef LEVELTESTING
    std::string m_filename;
#endif

    void doDraw(const DrawParameter& param) override;

    void createLabelAt(const Entity& target, const std::string& fontName, const int points);
    void createLabelAt(const Entity& target, const std::string& fontName, const std::string& text);
    void createLabelAt(const sf::Vector2f& position, const std::string& fontName, const std::string& text);
    void respawnDeadBalls();
    void trackBallMovement(double elapsedTime);
    void spawnPendingEntities(double currentTime);
    void cleanupKilledEntities();
    void updatePointLabels();
    void updateGoodyChoice();
    void updateGoodyCharges();

    bool onShouldCollide(Entity& entityA, Entity& entityB) override;
    void handleCollision(Entity& entityA, Entity& entityB, const b2Vec2& point, const float impulse) override;
    void killTarget(Entity& target);
    void killBonusTarget(Entity& target);
    void prepareEntityForSpawn(const b2Vec2& position, const Entity& spawn, float angle = 0);

    void handleAutoRespawn();

    /// Load the level after m_number
    LevelInfo load(bool loadInfoOnly = false);

    std::unique_ptr<Entity> parseEntity(
        const tinyxml2::XMLElement* xml,
        const std::string& templateName,
        Templates& templates);

    void parsePreview(const tinyxml2::XMLElement& levelinfo, LevelInfo& infoToFill);
    void parseGameplayAttributes(const tinyxml2::XMLElement& gameplay, LevelInfo& infoToFill);

    /// Construct the full level filename from the level number
    std::string filename();
    std::string pathname() const;

    bool isOriginal();

private:
    void onRestarted() override;

    double onGetValueOf(const std::string& name) const override;
    void onSetValueOf(const std::string& name, const double value) override;

    std::unique_ptr<ResourceManager> m_resourceManager;
    AppConfig& m_config;

    b2World m_world;
    b2Vec2 m_defaultGravity;
    b2Vec2 m_gravity;
    float m_gravityFactor;

    CloneHandler m_cloneHandler;

    unsigned int m_number;

    std::map<std::string, double> m_variables;
    Entity* m_updatingEntity;
    std::vector<std::unique_ptr<Entity>> m_entities;
    std::vector<std::unique_ptr<Entity>> m_entitiesToSpawn;
    std::vector<EntitySpawn> m_unspawnedEntities;

    std::unique_ptr<Background> m_background;

    ContactListener  m_contactListener;

    double m_timeStep;
    int m_velocityIterations;
    int m_positionIterations;

    float m_width;
    float m_height;

    double m_lastTime;
    double m_levelEndingTime;
    double m_levelPlayTime;

#ifdef _DEBUG_BOX2D
    bool m_debugDraw;
#endif

    ScrollView m_scrollView;

    Ball* m_ball;
    int m_remainingBall;
    int m_lostBallCounter;

    // For trackBallMovement().
    struct BallMovement
    {
        BallMovement() : time(0.0f), distance(0.0f) {}
        BallMovement(double time, float dist) : time(time), distance(dist) {}
        double time;
        float distance;
    };
    std::queue<BallMovement> m_ballTravelDistances;
    double m_ballImpulseTime;
    b2Vec2 m_lastBallPosition;
    float m_ballTravelDistance;
    RandomProvider m_ballImpulseAngle;

    int m_totalTarget;
    int m_remainingTarget;

    int m_points;
    int m_bonusTargetPoints;
    int m_normalTargetPoints;
    int m_multiHit;

    double m_remainingTime;
    double m_totalTime;
    double m_initialTime;
    unsigned int m_defaultTargetBuffer;

    struct TimedLabel
    {
    public:

        TimedLabel(std::unique_ptr<LineLabel> label, const double time)
        {
            this->label = std::move(label);
            this->time = time;
        }

        std::unique_ptr<LineLabel> label;
        double time;
    };

    std::vector<std::unique_ptr<TimedLabel>> m_pointLabels;

    bool m_levelPass;

    bool m_timeAttackMode;

    int m_goldMedal;
    int m_silverMedal;
    int m_bronzeMedal;
    Medals m_currentMedal;

    std::string m_levelName;

    GravityGoody m_gravityGoody;
    InvulnerableGoody m_invulnerableGoody;
    Goody m_extraBallGoody;
    Goody m_extraTimeGoody;
    int m_currentSeletedGoody;
    bool m_playing;

    GameEventRecorder m_eventRecorder;
    LevelInfo m_info;
};

#endif // LEVEL_HPP
