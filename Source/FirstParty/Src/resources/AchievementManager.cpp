#include "AchievementManager.hpp"
#include "PathHelper.hpp"
#include <tinyxml2.h>
#include "../Utility.hpp"
#include "../resources/AppConfig.hpp"

unsigned int AchievementManager::Points_At_Once_Step = 0;
unsigned int AchievementManager::Points_At_Once_MaxSteps = 0;
unsigned int AchievementManager::Stars_At_Once_Max = 0;
unsigned int AchievementManager::Level_Tam_Min_Of_Time_Step = 0;
unsigned int AchievementManager::Level_Tam_Min_Of_Time_MaxSteps = 0;
unsigned int AchievementManager::Level_Nam_Max_Of_Time_Step = 0;
unsigned int AchievementManager::Level_Nam_Max_Of_Time_MaxSteps = 0;
unsigned int AchievementManager::Max_Loop_Counter = 0;

AchievementManager::AchievementManager(const std::string& file, AppConfig& config) :
    m_achievementFile(file),
    m_config(config)
{
    if(!load())
        throw std::runtime_error(utility::replace(utility::translateKey("@AchievementLoadError"), m_achievementFile));

    loadValues();
}

AchievementManager::~AchievementManager()
{
    saveValues();
}

void AchievementManager::addAchievement(const std::string& name,
                                        Achievement::Condition condition,
                                        Achievement::Specifically specifically,
                                        Achievement::Object object,
                                        unsigned int bronze,
                                        unsigned int silver,
                                        unsigned int gold,
                                        unsigned int atOnceAmount)
{
    std::string id = Achievement::createId(condition, specifically, object, atOnceAmount);

    for(auto it = m_achievements.begin(); it != m_achievements.end(); ++it)
        if(it->first == id)
            throw std::runtime_error(utility::replace(utility::translateKey("@DoubleAchievement"), id));
    
    m_achievements.push_back(std::make_pair(id, Achievement(name, condition, specifically, object, bronze, silver, gold, atOnceAmount)));
}

bool AchievementManager::load()
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile((resourcePath() + "res/" + m_achievementFile).c_str());

    if(doc.Error()) // Error while loading file
    {
        doc.PrintError();
        return false;
    }

    loadSetup(doc);

    if(auto achievements = doc.FirstChildElement("achievements"))
    {
        unsigned int specificallyAmount = 0;
        for(auto achievement = achievements->FirstChildElement("achievement");
            achievement != nullptr; achievement = achievement->NextSiblingElement("achievement"))
        {
            specificallyAmount = 0;
            std::string key = achievement->Attribute("key");
            Achievement::Condition condition = stringToCondition(achievement->Attribute("condition"));
            Achievement::Object object= stringToObject(achievement->Attribute("object"));
            Achievement::Specifically specifically = Achievement::Nothing;
            std::string specificallyAsString = achievement->Attribute("specifically");
            if(specificallyAsString.find("=") != std::string::npos)
            {
                std::size_t pos = specificallyAsString.find("=") + 1; 
                std::string amount = specificallyAsString.substr(pos);
                specificallyAmount = utility::stringTo<unsigned int>(amount);
                if(specificallyAsString.find("AtOnce") != std::string::npos)
                    specifically = stringToSpecifically("AtOnce");
                else if(specificallyAsString.find("MinOfTime") != std::string::npos)
                    specifically = stringToSpecifically("MinOfTime");
                else if(specificallyAsString.find("MaxOfTime") != std::string::npos)
                    specifically = stringToSpecifically("MaxOfTime");
                else
                    throw std::runtime_error(utility::replace(utility::translateKey("@UnknownAchievementSpecifically"), specificallyAsString));
            }
            else
                specifically = stringToSpecifically(specificallyAsString);
            
            int gold = achievement->IntAttribute("gold");
            int silver = achievement->IntAttribute("silver");
            int bronze = achievement->IntAttribute("bronze");

            addAchievement(key, condition, specifically, object, bronze, silver, gold, specificallyAmount);
        }
    }

    return true;
}

void AchievementManager::loadSetup(tinyxml2::XMLDocument& doc)
{
    if(auto achievements = doc.FirstChildElement("achievements"))
    {
        auto config = achievements->FirstChildElement("config");

        Points_At_Once_Step = config->FirstChildElement("points")->IntAttribute("pointstep");
        Points_At_Once_MaxSteps = config->FirstChildElement("points")->IntAttribute("maxsteps");

        Stars_At_Once_Max = config->FirstChildElement("stars")->IntAttribute("max");

        Level_Tam_Min_Of_Time_Step = config->FirstChildElement("leveltam")->IntAttribute("timestep");
        Level_Tam_Min_Of_Time_MaxSteps = config->FirstChildElement("leveltam")->IntAttribute("maxsteps");

        Level_Nam_Max_Of_Time_Step = config->FirstChildElement("levelnam")->IntAttribute("timestep");
        Level_Nam_Max_Of_Time_MaxSteps = config->FirstChildElement("levelnam")->IntAttribute("maxsteps");
    }

    if(Max_Loop_Counter < Points_At_Once_MaxSteps)
        Max_Loop_Counter = Points_At_Once_MaxSteps;

    if(Max_Loop_Counter < Level_Tam_Min_Of_Time_MaxSteps)
        Max_Loop_Counter = Level_Tam_Min_Of_Time_MaxSteps;

    if(Max_Loop_Counter < Level_Nam_Max_Of_Time_MaxSteps)
        Max_Loop_Counter = Level_Nam_Max_Of_Time_MaxSteps;
}

Achievement::Object AchievementManager::stringToObject(const std::string& object)
{
    if(object == "Target")
        return Achievement::Target;
    else if(object == "BonusTarget")
        return Achievement::BonusTarget;
    else if(object == "GravityGoody")
        return Achievement::GravityGoody;
    else if(object == "InvulnerableGoody")
        return Achievement::InvulnerableGoody;
    else if(object == "ExtraBallGoody")
        return Achievement::ExtraBallGoody;
    else if(object == "ExtraTimeGoody")
        return Achievement::ExtraTimeGoody;
    else if(object == "Coins")
        return Achievement::Coins;
    else if(object == "Level")
        return Achievement::Level;
    else if(object == "LevelNAM")
        return Achievement::LevelNAM;
    else if(object == "LevelTAM")
        return Achievement::LevelTAM;
    else if(object == "Ball")
        return Achievement::Ball;
    else if(object == "GoodyGenaral")
        return Achievement::GoodyGenaral;
    else if(object == "Stars")
        return Achievement::Stars;
    else
        throw std::runtime_error(utility::replace(utility::translateKey("@UnknownAchievementObject"), object));
}

Achievement::Condition AchievementManager::stringToCondition(const std::string& condition)
{
    if(condition == "Collect")
        return Achievement::Collect;
    else if(condition == "Hit")
        return Achievement::Hit;
    else if(condition == "Buy")
        return Achievement::Buy;
    else if(condition == "Sell")
        return Achievement::Sell;
    else if(condition == "Lose")
        return Achievement::Lose;
    else if(condition == "Finish")
        return Achievement::Finish;
    else if(condition == "Use")
        return Achievement::Use;
    else 
        throw std::runtime_error(utility::replace(utility::translateKey("@UnknownAchievementCondition"), condition));
}

Achievement::Specifically AchievementManager::stringToSpecifically(const std::string& specifically)
{
    if(specifically == "Nothing")
        return Achievement::Nothing;
    else if(specifically == "InSum")
        return Achievement::InSum;
    else if(specifically == "InCash")
        return Achievement::InCash;
    else if(specifically == "WhenSpeeding")
        return Achievement::WhenSpeeding;
    else if(specifically == "AtOnce")
        return Achievement::AtOnce;
    else if(specifically == "MinOfTime")
        return Achievement::MinOfTime;
    else if(specifically == "MaxOfTime")
        return Achievement::MaxOfTime;
    else
        throw std::runtime_error(utility::replace(utility::translateKey("@UnknownAchievementSpecifically"), specifically));
}

void AchievementManager::analyseGameEvents(const std::list<GameEvent>& gameEvents)
{
    unsigned int hitTargetInARowCounter = 0;
    unsigned int hitBonusTargetInARowCounter = 0;
    bool gravityOn = false;
    bool invulnerableOn = false;

    for(auto it = std::begin(gameEvents); it != std::end(gameEvents); ++it)
    {
        if(it->getEventType() == GameEvent::HitTarget || it->getEventType() == GameEvent::HitTargetSpeeding)
        {
            addValueTo(Achievement::Hit, Achievement::InSum, Achievement::Target, 1);
            hitTargetInARowCounter++;
            if(it->getEventType() == GameEvent::HitTargetSpeeding)
                addValueTo(Achievement::Hit, Achievement::WhenSpeeding, Achievement::Target, 1);
        }

        if(it->getEventType() == GameEvent::HitBonusTarget || it->getEventType() == GameEvent::HitBonusTargetSpeeding)
        {
            addValueTo(Achievement::Hit, Achievement::InSum, Achievement::BonusTarget, 1);
            hitBonusTargetInARowCounter++;
            if(it->getEventType() == GameEvent::HitBonusTargetSpeeding)
                addValueTo(Achievement::Hit, Achievement::WhenSpeeding, Achievement::BonusTarget, 1);
        }
        
        if(it->getEventType() == GameEvent::GravityGoody && gravityOn == false)
        {
            addValueTo(Achievement::Use, Achievement::InSum, Achievement::GravityGoody, 1);
            addValueTo(Achievement::Use, Achievement::InSum, Achievement::GoodyGenaral, 1);
            gravityOn = true;
        }
        else if(it->getEventType() == GameEvent::GravityGoody && gravityOn == true)
            gravityOn = false;

        if(it->getEventType() == GameEvent::InvulnerableGoody && invulnerableOn == false)
        {
            addValueTo(Achievement::Use, Achievement::InSum, Achievement::InvulnerableGoody, 1);
            addValueTo(Achievement::Use, Achievement::InSum, Achievement::GoodyGenaral, 1);
            invulnerableOn = true;
        }
        else if(it->getEventType() == GameEvent::GravityGoody && invulnerableOn == true)
            invulnerableOn = false;

        if(it->getEventType() == GameEvent::ExtraBallGoody)
        {
            addValueTo(Achievement::Use, Achievement::InSum, Achievement::ExtraBallGoody, 1);
            addValueTo(Achievement::Use, Achievement::InSum, Achievement::GoodyGenaral, 1);
        }

        if(it->getEventType() == GameEvent::ExtraTimeGoody)
        {
            addValueTo(Achievement::Use, Achievement::InSum, Achievement::ExtraTimeGoody, 1);
            addValueTo(Achievement::Use, Achievement::InSum, Achievement::GoodyGenaral, 1);
        }

        if(it->getEventType() == GameEvent::HitTeeter)
        {
            for(unsigned int i = 2; i <= hitTargetInARowCounter; ++i)
                addValueTo(Achievement::Hit, Achievement::AtOnce, Achievement::Target, i, 1);
                
            hitTargetInARowCounter = 0;

            for(unsigned int i = 2; i <= hitBonusTargetInARowCounter; ++i)
                addValueTo(Achievement::Hit, Achievement::AtOnce, Achievement::BonusTarget, i, 1);

            hitBonusTargetInARowCounter = 0;
        }
    }
}

void AchievementManager::addValueTo(Achievement::Condition condition,
                                    Achievement::Specifically specifically,
                                    Achievement::Object object,
                                    unsigned int value)
{
    std::string id = Achievement::createId(condition, specifically, object, 0);

    for(auto it = m_achievements.begin(); it != m_achievements.end(); ++it)
    {
        if(it->first == id)
        {
            Achievement::AchievementLevel level = it->second.getAchievementLevel();
            it->second.addToCounter(value);
            if(level != it->second.getAchievementLevel())
                m_latestAchievementLevelUp.push_back(id);
        }
    } 
}

void AchievementManager::addValueTo(Achievement::Condition condition,
                                    Achievement::Specifically specifically,
                                    Achievement::Object object,
                                    unsigned int atOnceAmount,
                                    unsigned int value)
{
    std::string id = Achievement::createId(condition, specifically, object, atOnceAmount);

    for(auto it = m_achievements.begin(); it != m_achievements.end(); ++it)
    {
        if(it->first == id)
        {
            Achievement::AchievementLevel level = it->second.getAchievementLevel();
            it->second.addToCounter(value);
            if(level != it->second.getAchievementLevel())
                m_latestAchievementLevelUp.push_back(id);
        }
    }
}

void AchievementManager::setValueTo(Achievement::Condition condition,
                                    Achievement::Specifically specifically,
                                    Achievement::Object object,
                                    unsigned int value)
{
    std::string id = Achievement::createId(condition, specifically, object, 0);

    for(auto it = m_achievements.begin(); it != m_achievements.end(); ++it)
    {
        if(it->first == id)
        {
            Achievement::AchievementLevel level = it->second.getAchievementLevel();
            it->second.addToCounter(value);
            if(level != it->second.getAchievementLevel())
            m_latestAchievementLevelUp.push_back(id);
        }
    }
}

Achievement* AchievementManager::getAchievement(unsigned int number)
{
    if(number >= m_achievements.size())
        return nullptr;

    return &m_achievements[number].second;
}

Achievement* AchievementManager::getAchievement(const std::string& id)
{
    for(auto it = m_achievements.begin(); it != m_achievements.end(); ++it)
    {
        if(it->first == id)
            return &(it->second);
    }

    return nullptr;
}

void AchievementManager::saveValues()
{
    for(auto it = std::begin(m_achievements); it != std::end(m_achievements); ++it)
        m_config.set(it->second.getKey(), it->second.getCounter());
}

void AchievementManager::loadValues()
{
    for(auto it = std::begin(m_achievements); it != std::end(m_achievements); ++it)
        it->second.setCounter(m_config.get<unsigned int>(it->second.getKey()));
}

bool AchievementManager::newAchievementLevelUp() const
{
    return m_latestAchievementLevelUp.size() > 0 ;
}

std::string AchievementManager::popLatestAchievementLevelUp()
{
    std::string result = "";

    if(!m_latestAchievementLevelUp.empty())
    {
        result = m_latestAchievementLevelUp.back();
        m_latestAchievementLevelUp.pop_back();
    }

    return result;
}