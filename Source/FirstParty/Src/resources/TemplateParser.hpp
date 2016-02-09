#pragma once

#ifndef TEMPLATE_PARSER_HPP
#define TEMPLATE_PARSER_HPP

#include "tinyxml2.h"

#include <memory>
#include <unordered_map>
#include <vector>

struct Templates
{
    struct Objects
    {
        std::vector<const tinyxml2::XMLElement*> entities;
        std::vector<const tinyxml2::XMLElement*> backgrounds;

        Objects()
        { }

        Objects(Objects&& other) :
            entities(std::move(other.entities)),
            backgrounds(std::move(other.backgrounds))
        { }

        Objects& operator= (Objects&& other)
        {
            entities = std::move(other.entities);
            backgrounds = std::move(other.backgrounds);
            return *this;
        }
    };
    std::unordered_map<std::string, const tinyxml2::XMLElement*> physics;
    std::unordered_map<std::string, const tinyxml2::XMLElement*> shapes;
    std::unordered_map<std::string, const tinyxml2::XMLElement*> entities;
    std::unordered_map<std::string, const tinyxml2::XMLElement*> functions;
    std::unordered_map<std::string, const tinyxml2::XMLElement*> overrides;
    std::vector<std::unique_ptr<tinyxml2::XMLDocument>> documents;
    Objects objects;

    Templates()
    { }

    Templates(Templates&& other) :
        physics(std::move(other.physics)),
        shapes(std::move(other.shapes)),
        entities(std::move(other.entities)),
        functions(std::move(other.functions)),
        overrides(std::move(other.overrides)),
        documents(std::move(other.documents))
    { }

    Templates& operator= (Templates&& other)
    {
        physics = std::move(other.physics);
        shapes = std::move(other.shapes);
        entities = std::move(other.entities);
        functions = std::move(other.functions);
        overrides = std::move(other.overrides);
        documents = std::move(other.documents);
        return *this;
    }
};

class TemplateParser
{
public:
    TemplateParser(std::string subPath) : 
        m_subPath(subPath)
    { }

    Templates parse(const tinyxml2::XMLElement& xml);
    void parseObjects(const tinyxml2::XMLElement& xml, Templates& templates);

private:
    void parse(const tinyxml2::XMLElement& xml, Templates& templates);
    void parseIncludes(const tinyxml2::XMLElement& xml, Templates& templates);
    void parseShapes(const tinyxml2::XMLElement& xml, Templates& templates);
    void parsePhysics(const tinyxml2::XMLElement& xml, Templates& templates);
    void parseFunctions(const tinyxml2::XMLElement& xml, Templates& templates);
    void parseEntities(const tinyxml2::XMLElement& xml, Templates& templates);
    void parseOverrides(const tinyxml2::XMLElement& xml, Templates& templates);

    std::string m_subPath;
};

#endif // TEMPLATE_PARSER_HPP
