#pragma once

#ifndef MENU_LOADER_HPP
#define MENU_LOADER_HPP

#include "../animation/CloneHandler.hpp"
#include "../gui/Button.hpp"
#include "../gui/CheckBox.hpp"
#include "../gui/InputBox.hpp"
#include "../gui/InteractiveLabel.hpp"
#include "../gui/LineLabel.hpp"
#include "../gui/MenuSprite.hpp"
#include "../gui/MenuTemplate.hpp"
#include "../gui/Slider.hpp"
#include "../gui/SubWindow.hpp"
#include "../gui/ToolTip.hpp"
#include "../model/SoundObject.hpp"

#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

class ResourceManager;

class MenuLoader
{
public:

    static MenuTemplate* loadMenuTemplate(const std::string& path, ResourceManager& resourceManager);

private:

    MenuLoader()
    {
    }

    static CloneHandler _cloneHandler;

    static std::vector<std::unique_ptr<Button>> parseButtons(
        const tinyxml2::XMLElement* menuXml, 
        const std::unordered_map<std::string, ButtonStyle>& buttonStyles, 
        const std::unordered_map<std::string, ToolTip>& toolTip,
        ResourceManager& resourceManager);

    static std::vector<std::unique_ptr<CheckBox>> parseCheckBoxes(
        const tinyxml2::XMLElement* menuXml, 
        const std::unordered_map<std::string, CheckBoxStyle>& checkBoxStyles,
        const std::unordered_map<std::string, ToolTip>& toolTip,
        ResourceManager& resourceManager);

    static std::vector<std::unique_ptr<Slider>> parseSliders(
        const tinyxml2::XMLElement* menuXml, 
        const std::unordered_map<std::string, SliderStyle>& sliderStyles, 
        ResourceManager& resourceManager);

    static std::vector<std::unique_ptr<LineLabel>> parseLabels(
        const tinyxml2::XMLElement* menuXml, 
        ResourceManager& resourceManager);

    static std::vector<std::unique_ptr<InteractiveLabel>> parseInteractiveLabels(
        const tinyxml2::XMLElement* menuXml,
        const std::unordered_map<std::string, ToolTip>& toolTip,
        ResourceManager& resourceManager);

    static std::vector<std::unique_ptr<MenuSprite>> parseImages(
        const tinyxml2::XMLElement* menuXml,
        const std::unordered_map<std::string, ToolTip>& toolTip,
        ResourceManager& resourceManager);

    static std::vector<std::unique_ptr<InputBox>> parseInputBox(
        const tinyxml2::XMLElement* menuXml,
        const std::unordered_map<std::string, InputBoxStyle>& inputBoxStyle,
        ResourceManager& resourceManager);

    static std::vector<std::unique_ptr<SubWindow>> parseSubWindow(
        const tinyxml2::XMLElement* menuXml,
        ResourceManager& resourceManager,
        const std::unordered_map<std::string, ToolTip>& toolTip,
        const std::unordered_map<std::string, SliderStyle>& sliderStyles,
        const std::unordered_map<std::string, CheckBoxStyle>& checkBoxStyles,
        const std::unordered_map<std::string, ButtonStyle>& buttonStyles);

    static std::vector<std::unique_ptr<AnimationContainer>> parseAnimationContainer(
        const tinyxml2::XMLElement* menuXml,
        ResourceManager& resourceManager);

    static std::unordered_map<std::string, ButtonStyle> parseButtonStyles(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager);
    static std::unordered_map<std::string, SoundObject> parseSounds(tinyxml2::XMLElement* menuXml, SoundManager& soundManager);
    static std::unordered_map<std::string, CheckBoxStyle> parseCheckBoxStyles(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager);
    static std::unordered_map<std::string, SliderStyle> parseSliderStyles(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager);
    static std::unordered_map<std::string, ToolTip> parseToolTipStyle(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager);
    static std::unordered_map<std::string, InputBoxStyle> parseInputBoxStyle(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager);

    static ButtonStateStyle loadButtonStateStyle(tinyxml2::XMLElement* xml, ResourceManager& resourceManager);
    static CheckBoxStateStyle loadCheckBoxStateStyle(tinyxml2::XMLElement* xml, ResourceManager& resourceManager);
    static SliderStateStyle loadSliderStateStyle(tinyxml2::XMLElement* xml, ResourceManager& resourceManager);
};

#endif // MENU_LOADER_HPP
