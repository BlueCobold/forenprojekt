
#include "TemplateParser.hpp"

#include "PathHelper.hpp"
#include "ValueParser.hpp"

Templates TemplateParser::parse(const tinyxml2::XMLElement& xml) const
{
    Templates templates;
    parse(xml, templates);
    return templates;
}

void TemplateParser::parse(const tinyxml2::XMLElement& xml, Templates& templates) const
{
    if(auto shapes = xml.FirstChildElement("shapes"))
        parseShapes(*shapes, templates);

    if(auto physics = xml.FirstChildElement("physics"))
        parsePhysics(*physics, templates);

    if(auto functions = xml.FirstChildElement("functions"))
        parseFunctions(*functions, templates);

    if(auto entities = xml.FirstChildElement("entities"))
        parseEntities(*entities, templates);

    if(auto overrides = xml.FirstChildElement("overrides"))
        parseOverrides(*overrides, templates);

    parseIncludes(xml, templates);
}

void TemplateParser::parseShapes(const tinyxml2::XMLElement& xml, Templates& templates) const
{
    auto values = std::move(ValueParser::parseList(xml, "shape", "name"));
    templates.shapes.insert(begin(values), end(values));
}

void TemplateParser::parsePhysics(const tinyxml2::XMLElement& xml, Templates& templates) const
{
    auto values = std::move(ValueParser::parseList(xml, "physic", "name"));
    templates.physics.insert(begin(values), end(values));
}

void TemplateParser::parseFunctions(const tinyxml2::XMLElement& xml, Templates& templates) const
{
    auto values = std::move(ValueParser::parseList(xml, "function", "name"));
    templates.functions.insert(begin(values), end(values));
}

void TemplateParser::parseEntities(const tinyxml2::XMLElement& xml, Templates& templates) const
{
    // Add use keys 'name' (objects) and 'rep' (grid)
    auto reps = std::move(ValueParser::parseList(xml, "entity", "rep"));
    templates.entities.insert(begin(reps), end(reps));
    auto temp = std::move(ValueParser::parseList(xml, "entity", "name"));
    templates.entities.insert(begin(temp), end(temp));
}

void TemplateParser::parseOverrides(const tinyxml2::XMLElement& xml, Templates& templates) const
{
    auto values = std::move(ValueParser::parseList(xml, "override", "newRep"));
    templates.overrides.insert(begin(values), end(values));
    values = std::move(ValueParser::parseList(xml, "override", "newName"));
    templates.overrides.insert(begin(values), end(values));
}

void TemplateParser::parseObjects(const tinyxml2::XMLElement& xml, Templates& templates) const
{
    if(auto backgroundXml = xml.FirstChildElement("background"))
        templates.objects.backgrounds.push_back(backgroundXml);
        
    for(auto entity = xml.FirstChildElement("entity");
        entity != nullptr; entity = entity->NextSiblingElement("entity"))
        templates.objects.entities.push_back(entity);

    parseIncludes(xml, templates);
}

void TemplateParser::parseIncludes(const tinyxml2::XMLElement& xml, Templates& templates) const
{
    for(auto child = xml.FirstChildElement("include"); child != nullptr; child = child->NextSiblingElement("include"))
    {
        std::unique_ptr<tinyxml2::XMLDocument> doc(new tinyxml2::XMLDocument);
        std::string filename = resourcePath() + m_subPath + child->Attribute("file");
        doc->LoadFile(filename.c_str());
        if(doc->Error())
        {
            doc->PrintError();
            throw std::runtime_error(utility::replace(utility::translateKey("@InvalidXml"), filename));
        }

        if(auto root = doc->RootElement())
        {
            if(auto xmlTemplates = root->FirstChildElement("templates"))
                parse(*xmlTemplates, templates);
            
            if(auto xmlTemplates = root->FirstChildElement("objects"))
                parseObjects(*xmlTemplates, templates);
        }
        templates.documents.push_back(std::move(doc));
    }
}
