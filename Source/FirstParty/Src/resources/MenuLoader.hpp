#pragma once

#ifndef MENU_LOADER_HPP
#define MENU_LOADER_HPP

#include "../animation/CloneHandler.hpp"
#include "../gui/Button.hpp"
#include "../gui/Border.hpp"
#include "../gui/CheckBox.hpp"
#include "../gui/CustomContent.hpp"
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

    std::unique_ptr<MenuTemplate> loadMenuTemplate(const std::string& path);

    MenuLoader(ResourceManager& resourceManager)
        : m_resourceManager(resourceManager)
    { }

private:
    ResourceManager& m_resourceManager;
    static CloneHandler _cloneHandler;
    std::unordered_map<std::string, ToolTip> m_toolTips;
    std::unordered_map<std::string, SliderStyle> m_sliderStyles;
    std::unordered_map<std::string, CheckBoxStyle> m_checkBoxStyles;
    std::unordered_map<std::string, InputBoxStyle> m_inputBoxStyles;
    std::unordered_map<std::string, ButtonStyle> m_buttonStyles;
    std::unordered_map<std::string, BorderStyle> m_borderStyles;

    std::vector<std::unique_ptr<Button>> parseButtons(
        const tinyxml2::XMLElement& menuXml);

    std::vector<std::unique_ptr<Border>> parseBorders(
        const tinyxml2::XMLElement& menuXml);

    std::vector<std::unique_ptr<CustomContent>> parseCustomContents(
        const tinyxml2::XMLElement& menuXml);

    std::vector<std::unique_ptr<CheckBox>> parseCheckBoxes(
        const tinyxml2::XMLElement& menuXml);

    std::vector<std::unique_ptr<Slider>> parseSliders(
        const tinyxml2::XMLElement& menuXml);

    std::vector<std::unique_ptr<LineLabel>> parseLabels(
        const tinyxml2::XMLElement& menuXml);

    std::vector<std::unique_ptr<InteractiveLabel>> parseInteractiveLabels(
        const tinyxml2::XMLElement& menuXml);

    std::vector<std::unique_ptr<MenuSprite>> parseImages(
        const tinyxml2::XMLElement& menuXml);

    std::vector<std::unique_ptr<InputBox>> parseInputBox(
        const tinyxml2::XMLElement& menuXml);

    std::vector<std::unique_ptr<SubWindow>> parseSubWindow(
        const tinyxml2::XMLElement& menuXml);

    std::vector<std::unique_ptr<AnimationContainer>> parseAnimationContainer(
        const tinyxml2::XMLElement& menuXml);

    void parseButtonStyles(const tinyxml2::XMLElement& menuXml);
    void parseSounds(const tinyxml2::XMLElement& menuXml);
    void parseCheckBoxStyles(const tinyxml2::XMLElement& menuXml);
    void parseSliderStyles(const tinyxml2::XMLElement& menuXml);
    void parseToolTipStyles(const tinyxml2::XMLElement& menuXml);
    void parseInputBoxStyles(const tinyxml2::XMLElement& menuXml);
    void parseBorderStyles(const tinyxml2::XMLElement& menuXml);

    ButtonStateStyle loadButtonStateStyle(const tinyxml2::XMLElement& xml);
    CheckBoxStateStyle loadCheckBoxStateStyle(const tinyxml2::XMLElement& xml);
    SliderStateStyle loadSliderStateStyle(const tinyxml2::XMLElement& xml);
    void parseStyle(const tinyxml2::XMLElement& xml, BorderStyle& style);
};

#endif // MENU_LOADER_HPP
