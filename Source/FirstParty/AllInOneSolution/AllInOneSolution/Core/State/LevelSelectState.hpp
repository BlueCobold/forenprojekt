#pragma once

#ifndef LEVELSELECTSTATE_HPP
#define LEVELSELECTSTATE_HPP

#include "State.hpp"
#include "../gui/LevelSelectMenu.hpp"

class LevelSelectState : public State
{
public:

    LevelSelectState(sf::RenderWindow& screen, ResourceManager& resourceManager, AppConfig& config);
    ~LevelSelectState();

    virtual StateChangeInformation update(const float time);
    virtual void draw(const DrawParameter& params);
    virtual void onEnter(const EnterStateInformation* enterInformation, const float time);

private:
    LevelSelectMenu m_menu;
    std::map<int, std::string> m_levelNames;
    std::map<int, sf::IntRect> m_textureCoordinates;
    int m_currentLevelNumber;
    int m_unlockedLevel;

    void loadLevelInfos();
    EnterStateInformation m_stateInfo;
    EnterTransitionStateInformation m_transitionStateInfo;

    void updateLeftButton();
    void updateRightButton();
};

#endif // LEVELSELECTSTATE_HPP