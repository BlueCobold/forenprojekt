 #include "LevelPreviewState.hpp"

#include "../gui/Button.hpp"
#include "../gui/CheckBox.hpp"
#include "../model/Level.hpp"
#include "../resources/AppConfig.hpp"
#include "../rendering/transitions/RandomTransition.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

LevelPreviewState::LevelPreviewState(sf::RenderWindow& screen, 
                                 ResourceManager& resourceManager, 
                                 AppConfig& config,
                                 LoadCallback onLevelLoaded) :
    State(screen, resourceManager, config),
    m_menu(screen, resourceManager),
    m_HUD(resourceManager, config),
    m_level(nullptr),
    m_loadedLevel(nullptr),
    m_levelUpdated(false),
    m_transitionStateInfo(LevelPreviewStateId),
    m_optionStateInfo(LevelPreviewStateId),
    m_onLevelLoaded(std::move(onLevelLoaded))
{
    m_levelLoaderJob = std::unique_ptr<BackgroundLoader<LevelPreviewState>>(new BackgroundLoader<LevelPreviewState>(&LevelPreviewState::loadLevel, *this));
    loadLevelInfos();
    m_unlockedLevel = config.get<int>("UnlockedLevel");
    updateButton(LevelPreviewMenu::BUTTON_LEFT, m_unlockedLevel - 1);
    updateButton(LevelPreviewMenu::BUTTON_RIGHT, m_unlockedLevel + 1);
    auto captionName = m_levelInfos.find(1);
    if(captionName != end(m_levelInfos))
        m_menu.getLabel(LevelPreviewMenu::LABEL_LEVELNAME).setText(captionName->second.name);

    m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TIMEATTACKMODE).setChecked(false);
    m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TARGETMODE).setChecked(true);
}

LevelPreviewState::~LevelPreviewState()
{
}

void LevelPreviewState::loadLevel()
{
    try
    {
        m_loadingErrorMessage = std::string();
        int number = std::min(static_cast<int>(getResourceManager().getFileNames().size()), m_nextLevelNumber);
        auto tmp = std::unique_ptr<Level>(new Level("", number, getResourceManager().getSubScope("level"), m_config));
        tmp->restartAt(0);
        tmp->update(0);
        getResourceManager().purge("level");
        m_loadedLevel = std::move(tmp);
    }
    catch(std::bad_alloc& e)
    {
        m_loadingErrorMessage = e.what();
    }
    catch(std::runtime_error& e)
    {
        m_loadingErrorMessage = e.what();
    }
}

void LevelPreviewState::onEnter(const EnterStateInformation* enterInformation, const double time)
{
    m_levelUpdated = false;
    State::onEnter(enterInformation, time);

    m_level = enterInformation->m_level;

    m_HUD.onEnter(m_level);
    
    updateButton(LevelPreviewMenu::BUTTON_LEFT, m_level->number() - 1);
    updateButton(LevelPreviewMenu::BUTTON_RIGHT, m_level->number() + 1);
    auto& info = m_levelInfos.find(m_level->number())->second;
    m_menu.setLoading(false);
    m_menu.setLevelInfo(info.name, info.time, info.maxBalls);
    m_menu.setLevelPreview(info.preview);
    m_menu.setCoinToolTipText("tooltip_coins", utility::toString(m_config.get<int>("coins")));

    if(m_config.get<unsigned int>("UnlockedLevel") == m_level->number())
    {
        m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TIMEATTACKMODE).setToolTipText("tooltip_preview_notimeattack");
        m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TIMEATTACKMODE).setChecked(false);
        m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TARGETMODE).setChecked(true);
    }
    else
        m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TIMEATTACKMODE).setToolTipText("tooltip_preview_timeattack");

    m_menu.update(m_screen, getPassedTime());
}

StateChangeInformation LevelPreviewState::update(const double time)
{
    if(State::isPaused())
        return StateChangeInformation::Empty();

    if(m_loadedLevel)
    {
        if(!m_loadingErrorMessage.empty())
            throw std::runtime_error(m_loadingErrorMessage.c_str());

        m_level = m_loadedLevel.get();
        m_onLevelLoaded(m_loadedLevel);
    }

    int clicked = -1;
    updateTime(time);
    m_menu.setLoading(m_levelLoaderJob->isLoading());
    updateButton(LevelPreviewMenu::BUTTON_LEFT, m_level->number() - 1);
    updateButton(LevelPreviewMenu::BUTTON_RIGHT, m_level->number() + 1);
    m_menu.registerOnClick([&](const Button& sender){ clicked = sender.getId(); });
    m_menu.update(m_screen, getPassedTime());

    if(m_levelLoaderJob->isLoaded())
    {
        m_levelLoaderJob->reset();
        m_level->restartAt(time);
    }

    if(m_level->isTimeAttackMode())
    {
        if(m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TARGETMODE).getChecked())
        {
            m_level->setTimeAttackMode(false);
            m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TIMEATTACKMODE).setChecked(false);
        }
        else
            m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TIMEATTACKMODE).setChecked(true);
    }
    else
    {
        if(m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TIMEATTACKMODE).getChecked() &&
           m_config.get<unsigned int>("UnlockedLevel") > m_level->number())
        {
            m_level->setTimeAttackMode(true);
            m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TARGETMODE).setChecked(false);
        }
        else
        {
            m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TARGETMODE).setChecked(true);
            m_menu.getCheckbox(LevelPreviewMenu::CHECKBOX_TIMEATTACKMODE).setChecked(false);
        }
    }

    auto& info = m_levelInfos.find(m_level->number())->second;
    m_menu.setLevelInfo(info.name, info.time, info.maxBalls);
    m_menu.setLevelPreview(info.preview);

    if(m_levelLoaderJob->isLoading())
    {
        return StateChangeInformation::Empty();
    }
    else if(clicked == LevelPreviewMenu::BUTTON_LEFT)
    {
        if(m_levelInfos.find(m_level->number() - 1) != end(m_levelInfos))
        {
            m_nextLevelNumber = m_level->number() - 1;
            m_levelLoaderJob->run();
        }
    }
    else if(clicked == LevelPreviewMenu::BUTTON_RIGHT)
    {
        if(m_levelInfos.find(m_level->number() + 1) != end(m_levelInfos))
        {
            m_nextLevelNumber = m_level->number() + 1;
            m_levelLoaderJob->run();
        }
    }
    else if(clicked == LevelPreviewMenu::BUTTON_START)
    {
        m_level->setTimeAttackMode(m_menu.getCheckbox(10).getChecked());
        m_playStateInfo.m_prepareOnly = false;
        m_playStateInfo.m_returnFromPause = false;
        m_playStateInfo.m_level = m_level;
        m_playStateInfo.m_levelNumber = m_level->number();
        m_transitionStateInfo.m_followingState = PlayStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == LevelPreviewMenu::BUTTON_MAIN_MENU)
    {
        m_playStateInfo.m_prepareOnly = false;
        m_playStateInfo.m_returnFromPause = false;
        m_playStateInfo.m_level = m_level;
        m_playStateInfo.m_levelNumber = m_level->number();
        m_transitionStateInfo.m_followingState = MainMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_playStateInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == LevelPreviewMenu::BUTTON_OPTIONS)
    {
        m_optionStateInfo.m_level = m_level;
        m_optionStateInfo.m_prepareOnly = false;
        m_transitionStateInfo.m_level = m_level;
        m_transitionStateInfo.m_followingState = OptionMenuStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_optionStateInfo;
        m_transitionStateInfo.m_comeFromeState = LevelPreviewStateId;
        m_transitionStateInfo.m_transitionType = RandomTransition::TypeCount;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == LevelPreviewMenu::BUTTON_COINS)
    {
        m_coinShopStateInfo.m_level = m_level;
        m_coinShopStateInfo.m_prepareOnly = false;
        m_coinShopStateInfo.m_comeFromState = LevelPreviewStateId;
        m_coinShopStateInfo.m_levelNumber = m_level->number();
        m_transitionStateInfo.m_followingState = CoinShopStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_coinShopStateInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }
    else if(clicked == LevelPreviewMenu::BUTTON_HIGHSCORE)
    {
        m_highScoreInfo.m_level = m_level;
        m_highScoreInfo.m_comeFromState = LevelPreviewStateId;
        m_highScoreInfo.m_levelNumber = m_level->number();
        m_transitionStateInfo.m_followingState = HighScoreStateId;
        m_transitionStateInfo.m_onEnterInformation = &m_highScoreInfo;
        return StateChangeInformation(TransitionStateId, &m_transitionStateInfo);
    }

    return StateChangeInformation::Empty();
}

void LevelPreviewState::doDraw(const DrawParameter& params)
{
    if(m_level != nullptr)
    {
        if(!m_levelUpdated)
        {
            m_level->update(0);
            m_levelUpdated = true;
        }
        m_HUD.update(m_level, getPassedTime());
        m_level->adjustView(params);
        m_level->draw(params);
        m_HUD.draw(params);
    }

    params.getTarget().setView(utility::getDefaultView(params.getTarget(), m_screen.getSize()));

    sf::RectangleShape whiteRect;
    whiteRect.setSize(m_screen.getView().getSize());
    whiteRect.setFillColor(sf::Color(255, 255, 255, 128));
    params.getTarget().draw(whiteRect);

    m_menu.draw(params);
}

void LevelPreviewState::loadLevelInfos()
{
    tinyxml2::XMLDocument doc;
    std::string file = "";
    std::unordered_map<int, std::string> fileNames = State::getResourceManager().getFileNames();

    for(auto it = std::begin(fileNames); it != std::end(fileNames); ++it)
    {
        Level level("", it->first, getResourceManager().getSubScope("menus"), getConfig(), true);
        m_levelInfos[it->first] = level.getInfo();
    }
    getResourceManager().purge("menus");
}

void LevelPreviewState::updateButton(int id, int levelNumber)
{
    m_unlockedLevel = getConfig().get<int>("UnlockedLevel");
    auto preview = m_levelInfos.find(levelNumber);
    auto& b = m_menu.getButton(id);
    if(preview != end(m_levelInfos) && (m_unlockedLevel >= levelNumber))
    {
        auto& sprite = preview->second.preview;
        b.changeIdleSprite(sprite);
        b.changeHoverSprite(sprite);
        b.changePressedSprite(sprite);
        b.setVisible(true);
    }
    else
        b.setVisible(false);
}
