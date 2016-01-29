
#include "ValueParser.hpp"

#include "SpriteSheet.hpp"
#include "../animation/VariableHandler.hpp"

void ValueParser::parseConstants(const tinyxml2::XMLElement& xml, VariableHandler& target)
{
    std::map<std::string, std::unique_ptr<ValueProvider>> map;
    for(auto var = xml.FirstAttribute(); var != nullptr; var = var->Next())
        target.setValueOf(var->Name(), var->FloatValue());
}

std::vector<sf::Vector2i> ValueParser::parseValueList(const tinyxml2::XMLElement& xml, const std::string& x, const std::string& y)
{
    std::vector<sf::Vector2i> result;
    auto xxml = xml.FirstChildElement(x.c_str());
    auto yxml = xml.FirstChildElement(y.c_str());
    if(!xxml || !yxml)
        return result;

    std::istringstream tokens(xxml->GetText());
    std::string token;
    while(std::getline(tokens, token, ','))
        result.push_back(sf::Vector2i(utility::stringTo<int>(token), 0));

    tokens.str(yxml->GetText());
    size_t i = 0;
    while(std::getline(tokens, token, ','))
    {
        if(i > result.size())
            throw std::runtime_error(utility::replace(utility::translateKey("InvalidLayout"), y));
        result[i].y = utility::stringTo<int>(token);
        ++i;
    }
    if(i != result.size())
        throw std::runtime_error(utility::replace(utility::translateKey("InvalidLayout"), y));
    return result;
}

std::unordered_map<std::string, const tinyxml2::XMLElement*> ValueParser::parseList(
     const tinyxml2::XMLElement& xml,
     const std::string& attributeName,
     const std::string& key)
{
    std::unordered_map<std::string, const tinyxml2::XMLElement*> tempList;

    for(auto iterator = xml.FirstChildElement(attributeName.c_str());
       iterator != nullptr; iterator = iterator->NextSiblingElement(attributeName.c_str()))
    {
        if(auto nodeName = iterator->Attribute(key.c_str()))
            tempList.insert(std::make_pair(std::string(nodeName), iterator));
    }

    return std::move(tempList);
}

void ValueParser::parseSpriteValueList(const tinyxml2::XMLElement& xml,
    const SpriteSheet& sheet,
    std::vector<sf::Vector2i>& src,
    std::vector<sf::Vector2i>& offset,
    std::vector<sf::Vector2i>& sizes,
    std::vector<sf::Vector2i>& origins)
{
    std::istringstream tokens(xml.GetText());
    std::string token;
    while(std::getline(tokens, token, ','))
    {
        std::stringstream trimmer;
        trimmer << token;
        token.clear();
        trimmer >> token;
        auto sprite = sheet.get(token);
        src.push_back(sf::Vector2i(sprite.x, sprite.y));
        sizes.push_back(sf::Vector2i(sprite.width, sprite.height));
        offset.push_back(sf::Vector2i(sprite.offsetX, sprite.offsetY));
        origins.push_back(sf::Vector2i(static_cast<int>(sprite.originX), static_cast<int>(sprite.originY)));
    }
}