
#include "ControllerParser.hpp"
#include "../animation/provider/StaticProvider.hpp"
#include "../animation/provider/ValueProvider.hpp"
#include "../Utility.hpp"

Array<std::unique_ptr<ValueProvider>, 4> ControllerParser::parseColor(const tinyxml2::XMLElement& xml)
{
    Array<std::unique_ptr<ValueProvider>, 4> colors;
    std::unique_ptr<ValueProvider> red, green, blue, alpha, tmp;
    auto static_color_element = xml.FirstChildElement("color");
    const char* value = nullptr;
    while(static_color_element && !(value = static_color_element->Attribute("value")))
        static_color_element = static_color_element->NextSiblingElement("color");

    if(static_color_element && value)
    {
        sf::Color col = utility::hexToColor(value);
        colors[0] = std::unique_ptr<ValueProvider>(new StaticProvider(col.r / 255.f));
        colors[1] = std::unique_ptr<ValueProvider>(new StaticProvider(col.g / 255.f));
        colors[2] = std::unique_ptr<ValueProvider>(new StaticProvider(col.b / 255.f));
        colors[3] = std::unique_ptr<ValueProvider>(new StaticProvider(col.a / 255.f));
    }

    tmp = findController(xml, "color", "channel", "red");
    if(tmp != nullptr)
        colors[0] = std::move(tmp);

    tmp = findController(xml, "color", "channel", "green");
    if(tmp != nullptr)
        colors[1] = std::move(tmp);

    tmp = findController(xml, "color", "channel", "blue");
    if(tmp != nullptr)
        colors[2] = std::move(tmp);

    tmp = findController(xml, "color", "channel", "alpha");
    if(tmp != nullptr)
        colors[3] = std::move(tmp);

    return colors;
}

Array<std::unique_ptr<ValueProvider>, 2> ControllerParser::parsePosition(const tinyxml2::XMLElement& xml)
{
    Array<std::unique_ptr<ValueProvider>, 2> position;
    position[0] = findController(xml, "position", "axis", "x");
    position[1] = findController(xml, "position", "axis", "y");
    return position;
}

Array<std::unique_ptr<ValueProvider>, 2> ControllerParser::parseScale(const tinyxml2::XMLElement& xml)
{
    Array<std::unique_ptr<ValueProvider>, 2> scale;
    scale[0] = findController(xml, "scale", "axis", "x");
    scale[1] = findController(xml, "scale", "axis", "y");
    return std::move(scale);
}

std::unique_ptr<ValueProvider> ControllerParser::parseRotation(const tinyxml2::XMLElement& xml)
{
    if (auto rotation = xml.FirstChildElement("rotation"))
    {
        if(auto child = rotation->FirstChildElement())
            return m_providerParser.parseSingle(*child);
    }
    return nullptr;
}

std::unique_ptr<ValueProvider> ControllerParser::findController(
    const tinyxml2::XMLElement& xml,
    const std::string& childName,
    const std::string& propertyName,
    const std::string& propertyValue)
{
    for(auto iterator = xml.FirstChildElement(childName.c_str());
        iterator != nullptr; iterator = iterator->NextSiblingElement(childName.c_str()))
    {
        auto value = iterator->Attribute(propertyName.c_str());
        if(value == nullptr || value != propertyValue)
            continue;

        if(iterator->FirstChildElement() == nullptr)
            throw std::runtime_error(utility::translateKey("SubTag"));

        return m_providerParser.parseSingle(*iterator->FirstChildElement());
    }
    return nullptr;
}
