#pragma once

#ifndef ACHIEVEMENT_HPP
#define ACHIEVEMENT_HPP

#include <string>

class Achievement
{
public:

	enum AchievementLevel
	{
		Gold,
		Silver,
		Bronze,
		None
	};

    enum Condition
    {
        Collect,
        Hit,
        Buy,
        Sell,
        Lose,
        Finish,
        Use
    };

    enum Specifically
    {
        Nothing,
        InSum,
        InCash,
        WhenSpeeding,
        AtOnce,
        MinOfTime,
        MaxOfTime
    };

    enum Object
    {
        Target,
        BonusTarget,
        GravityGoody,
        InvulnerableGoody,
        ExtraBallGoody,
        ExtraTimeGoody,
        Coins,
        Level,
        LevelNAM,
        LevelTAM,
        Ball,
        GoodyGenaral,
        Stars
    };

    Achievement(const std::string& key,
                Condition condition,
                Specifically specifically,
                Object object,
                unsigned int bronze,
                unsigned int silver,
                unsigned int gold,
                unsigned int atOnceAmount = 0);

    void setCounter(const unsigned int counter);
    void addToCounter(const unsigned int counter);
    const unsigned int getCounter() const;
    const unsigned int getSpecificallyAmount() const;
	const AchievementLevel getAchievementLevel() const;
    const Condition getCondition() const;
    const Specifically getSpecifically() const;
    const Object getObject() const;
    const bool isBronze() const;
    const bool isSilver() const;
    const bool isGold() const;
    const std::string getId() const;
    const std::string getName() const;
    const std::string getKey() const;

    static std::string createId(Achievement::Condition condition,
                                Achievement::Specifically specifically,
                                Achievement::Object object,
                                unsigned int specificallyAmount);
private:
    const std::string m_key;
    std::string m_id;
    const Condition m_condition;
    const Specifically m_specifically;
    const Object m_object;
    const unsigned int m_amountBronze;
    const unsigned int m_amountSilver;
    const unsigned int m_amountGold;
    unsigned int m_counter;
    const unsigned int m_specificallyAmount;
};

#endif
