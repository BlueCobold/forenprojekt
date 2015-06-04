#pragma once

#ifndef ACHIEVEMENTMANAGER_HPP
#define ACHIEVEMENTMANAGER_HPP

#include "Achievement.hpp"
#include "GameEvent.hpp"
#include <list>
#include <unordered_map>

namespace tinyxml2
{
    class XMLDocument;
}
class AppConfig;

class AchievementManager
{
public:
    static unsigned int Points_At_Once_Step;
    static unsigned int Points_At_Once_MaxSteps;
    static unsigned int Stars_At_Once_Max;
    static unsigned int Level_Tam_Min_Of_Time_Step;
    static unsigned int Level_Tam_Min_Of_Time_MaxSteps;
    static unsigned int Level_Nam_Max_Of_Time_Step;
    static unsigned int Level_Nam_Max_Of_Time_MaxSteps;
    static unsigned int Max_Loop_Counter;

    AchievementManager(const std::string& file, AppConfig& config);
    ~AchievementManager();

    void analyseGameEvents(const std::list<GameEvent>& gameEvents);

    void addValueTo(Achievement::Condition condition,
                    Achievement::Specifically specifically,
                    Achievement::Object object,
                    unsigned int value);

    void addValueTo(Achievement::Condition condition,
                    Achievement::Specifically specifically,
                    Achievement::Object object,
                    unsigned int atOnceAmount,
                    unsigned int value);

    void setValueTo(Achievement::Condition condition,
                    Achievement::Specifically specifically,
                    Achievement::Object object,
                    unsigned int value);

    const std::unordered_map<std::string, Achievement>::const_iterator beginIterator();
    const std::unordered_map<std::string, Achievement>::const_iterator endIterator();

    void saveValues();

private:
    void addAchievement(const std::string& name,
                        Achievement::Condition condition,
                        Achievement::Specifically specifically,
                        Achievement::Object object,
                        unsigned int bronze,
                        unsigned int silver,
                        unsigned int gold,
                        unsigned int atOnceAmount);

    bool load();
    void loadSetup(tinyxml2::XMLDocument& doc);
    void loadValues();

    Achievement::Object stringToObject(const std::string& object);
    Achievement::Condition stringToCondition(const std::string& condition);
    Achievement::Specifically stringToSpecifically(const std::string& specifically);

    std::string m_achievementFile;
    std::unordered_map<std::string, Achievement> m_achievements;

    AppConfig& m_config;
};

#endif