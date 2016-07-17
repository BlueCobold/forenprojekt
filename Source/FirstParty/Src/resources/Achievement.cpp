#include "Achievement.hpp"
#include "../Utility.hpp"

Achievement::Achievement(const std::string& key,
                         Condition condition,
                         Specifically specifically,
                         Object object,
                         unsigned int bronze,
                         unsigned int silver,
                         unsigned int gold,
                         unsigned int specificallyAmount) :
    m_key(key),
    m_condition(condition),
    m_specifically(specifically),
    m_object(object),
    m_amountBronze(bronze),
    m_amountSilver(silver),
    m_amountGold(gold),
    m_counter(0),
    m_specificallyAmount(specificallyAmount)
{
    m_id = createId(m_condition, m_specifically, m_object, m_specificallyAmount);
}

void Achievement::setCounter(const unsigned int counter)
{
    m_counter = counter;
}

void Achievement::addToCounter(const unsigned int counter)
{
    m_counter += counter;
}

const unsigned int Achievement::getCounter() const
{
    return m_counter;
}

const Achievement::Condition Achievement::getCondition() const
{
    return m_condition;
}

const Achievement::Specifically Achievement::getSpecifically() const
{
    return m_specifically;
}

const Achievement::Object Achievement::getObject() const
{
    return m_object;
}

const bool Achievement::isBronze() const
{
    return m_counter >= m_amountBronze;
}

const bool Achievement::isSilver() const
{
    return m_counter >= m_amountSilver;
}

const bool Achievement::isGold() const
{
    return m_counter >= m_amountGold;
}

const unsigned int Achievement::getSpecificallyAmount() const
{
    return m_specificallyAmount;
}

std::string Achievement::createId(Achievement::Condition condition,
                                  Achievement::Specifically specifically,
                                  Achievement::Object object,
                                  unsigned int specificallyAmount)
{
    std::string result = "ID000000";
    std::string conditionString = utility::toString(condition);
    std::string specificallyString = utility::toString(specifically); 
    std::string objectString = utility::toString(object);
    result.replace(4 - conditionString.size(), conditionString.size(), conditionString);
    result.replace(6 - specificallyString.size(), specificallyString.size(), specificallyString);
    result.replace(8 - objectString.size(), objectString.size(), objectString);
    result.append(utility::toString(specificallyAmount));
    return result;
}

const std::string Achievement::getId() const
{
    return m_id;
}

const std::string Achievement::getName(const std::string& language) const
{
    return utility::translateKey(language + m_key);
}

const std::string Achievement::getKey() const
{
    return m_key;
}