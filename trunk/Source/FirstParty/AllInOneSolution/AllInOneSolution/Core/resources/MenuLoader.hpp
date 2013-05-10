#pragma once

#ifndef MENU_LOADER_HPP
#define MENU_LOADER_HPP

#include "../gui/MenuTemplate.hpp"

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
        ResourceManager& resourceManager);

    static std::unordered_map<std::string, ButtonStyle> parseStyles(tinyxml2::XMLElement* menuXml, ResourceManager& resourceManager);

    static ButtonStateStyle loadButtonStateStyle(tinyxml2::XMLElement* xml, ResourceManager& resourceManager);
};

#endif // MENU_LOADER_HPP
