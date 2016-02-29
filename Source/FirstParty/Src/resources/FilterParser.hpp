#pragma once

#ifndef FILTER_PARSER_HPP
#define FILTER_PARSER_HPP

#include "FilterContext.hpp"
#include "TemplateParser.hpp"
#include "../model/Entity.hpp"
#include "../model/collision/filter/CollisionFilter.hpp"

#include <functional>

#include <tinyxml2.h>

class FilterParser
{
public:
    typedef std::function<Entity*(const std::string& templateName)> ParserFunc;

private:
    FilterContext m_context;
    Entity& m_target;
    const Templates& m_templates;
    ParserFunc m_parser;

public:
    FilterParser(FilterContext context,
                 Entity& target,
                 Templates& templates,
                 ParserFunc parser) :
        m_context(context),
        m_target(target),
        m_templates(templates),
        m_parser(parser)
    {
        if(m_parser == nullptr)
            throw std::runtime_error("empty parser in filter");
    }

    void parse(const tinyxml2::XMLElement& xml);

private:
    std::unique_ptr<CollisionFilter> getFilter(const tinyxml2::XMLElement& xml) const;
};

#endif // FILTER_PARSER_HPP
