#pragma once

#ifndef MENU_LOADER_HPP
#define MENU_LOADER_HPP

#include "../gui/MenuTemplate.hpp"
#include "../gui/ToolTip.hpp"
#include "../model/SoundObject.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

class ResourceManager;

class MenuLoader
{
public:

    static MenuTemplate* loadMenuTemplate(const std::string& path, ResourceManager& resourceManager);

private:

    MenuLoader()
    {
    }

    static void parseButtons(
        MenuTemplate& menu, 
        tinyxml2::XMLElement* menuXml, 
        std::unordered_map<std::string, ButtonStyle>& buttonStyles, 
        std::unordered_map<std::string, SoundObject>& buttonSounds,
        std::unordered_map<std::string, ToolTip>& toolTip,
        ResourceManager& resourceManager);

    static void parseCheckBoxes(
        MenuTemplate& menu, 
        tinyxml2::XMLElement* menuXml, 
        std::unordered_map<std::string, CheckBoxStyle>& checkBoxStyles, 
        ResourceManager& resourceManager);

    static void parseSliders(
        MenuTemplate& menu, 
        tinyxml2::XMLElement* menuXml, 
        std::unordered_map<std::string, SliderStyle>& sliderStyles, 
        ResourceManager& resourceManager);

    static void parseLabels(
        MenuTemplate& menu, 
        tinyxml2::XMLElement* menuXml, 
        ResourceManager& resourceManager);

    static void parseImages(
        MenuTemplate& menu, 
        tinyxml2::XMLElement* menuXml,
        std::unordered_map<std::string, ToolTip>& toolTip,
        ResourceManager& resourceManager);

    static std::unordered_map<std::string, ButtonStyle> parseButtonStyles(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager);
    static std::unordered_map<std::string, SoundObject> parseSounds(tinyxml2::XMLElement* menuXml, SoundManager& soundManager);
    static std::unordered_map<std::string, CheckBoxStyle> parseCheckBoxStyles(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager);
    static std::unordered_map<std::string, SliderStyle> parseSliderStyles(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager);
    static std::unordered_map<std::string, ToolTip> parseToolTipStyle(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager);

    static ButtonStateStyle loadButtonStateStyle(tinyxml2::XMLElement* xml, ResourceManager& resourceManager);
    static CheckBoxStateStyle loadCheckBoxStateStyle(tinyxml2::XMLElement* xml, ResourceManager& resourceManager);
    static SliderStateStyle loadSliderStateStyle(tinyxml2::XMLElement* xml, ResourceManager& resourceManager);
};

#endif // MENU_LOADER_HPP
