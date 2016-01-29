#pragma once

#ifndef VALUE_PARSER_HPP
#define VALUE_PARSER_HPP

#include "../animation/Animation.hpp"
#include "ProviderParserContext.hpp"
#include "ProviderParser.hpp"

#include <unordered_map>
#include <vector>

#include "tinyxml2.h"

class SpriteSheet;
class VariableHandler;

class ValueParser
{
public:
    ValueParser()
    { }

    static std::unordered_map<std::string, const tinyxml2::XMLElement*> parseList(
        const tinyxml2::XMLElement& xml,
        const std::string& name,
        const std::string& key);

    static void parseSpriteValueList(const tinyxml2::XMLElement& xml,
        const SpriteSheet& sheet,
        std::vector<sf::Vector2i>& src,
        std::vector<sf::Vector2i>& offset,
        std::vector<sf::Vector2i>& sizes,
        std::vector<sf::Vector2i>& origins);

    static std::vector<sf::Vector2i> parseValueList(
        const tinyxml2::XMLElement& xml,
        const std::string& x,
        const std::string& y);

    static void parseConstants(const tinyxml2::XMLElement& xml, VariableHandler& target);
};

#endif // VALUE_PARSER_HPP
