#pragma once

#ifndef COLLIDER_PARSER_HPP
#define COLLIDER_PARSER_HPP

#include "ColliderContext.hpp"
#include "TemplateParser.hpp"
#include "../model/collision/handler/CollisionHandler.hpp"

#include <functional>
#include <memory>

#include "tinyxml2.h"

class Entity;

class ColliderParser
{
public:
    typedef std::function<Entity*(const std::string& templateName)> ParserFunc;

private:
    ColliderContext m_context;
    Templates& m_templates;
    Entity& m_target;
    ParserFunc m_parser;

public:
    ColliderParser(ColliderContext context,
                   Entity& target,
                   Templates& templates,
                   ParserFunc parser) :
        m_context(context),
        m_target(target),
        m_templates(templates),
        m_parser(parser)
    { }

    std::unique_ptr<CollisionHandler> parse(const tinyxml2::XMLElement& xml);

private:
    std::unique_ptr<CollisionHandler> parseShowLabelHandler(const tinyxml2::XMLElement& xml);
};

#endif // COLLIDER_PARSER_HPP
