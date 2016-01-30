
#include "PhysicsParser.hpp"

#include "ControllerParser.hpp"
#include "../Utility.hpp"

// Helper function to compute the area of a given polygon
float computeArea(const std::vector<b2Vec2>& vertices)
{
    auto area = 0.0f;
    b2Vec2 origin(0.0f, 0.0f);
    for(size_t i = 0; i < vertices.size(); ++i)
    {
        b2Vec2 d1 = vertices[i] - origin;
        b2Vec2 d2 = vertices[(i + 1) % vertices.size()] - origin;

        area += 0.5f * b2Cross(d1, d2);
    }
    return area;
}

PhysicsParser::Physics PhysicsParser::parse(const tinyxml2::XMLElement& physic,
                                            const tinyxml2::XMLElement& shape) const
{
    auto physics = parseBody(physic);

    // TODO: check property
    auto name = std::string(shape.Attribute("type"));

    // Load shape
    if(name == "polygon") // Load polygon
    {
        std::vector<b2Vec2> vertices;
        // Iterate over the vertices
        for(auto vertexIterator = shape.FirstChildElement("vertex");
            vertexIterator != nullptr; vertexIterator = vertexIterator->NextSiblingElement("vertex"))
        {
            vertices.push_back(b2Vec2(utility::toMeter(vertexIterator->FloatAttribute("x")),
                utility::toMeter(vertexIterator->FloatAttribute("y"))));
        }
        // Construct the b2Shape
        std::unique_ptr<b2PolygonShape> ps(new b2PolygonShape);
        ps->Set(vertices.data(), static_cast<int>(vertices.size()));
        physics.shapes.push_back(std::move(ps));
    }
    else if(name == "complex_polygon") // Load polygon
    {
        for(auto polyIterator = shape.FirstChildElement("polygon");
            polyIterator != nullptr; polyIterator = polyIterator->NextSiblingElement("polygon"))
        {
            std::vector<b2Vec2> vertices;
            // Iterate over the vertices
            for(auto vertexIterator = polyIterator->FirstChildElement("vertex");
                vertexIterator != nullptr; vertexIterator = vertexIterator->NextSiblingElement("vertex"))
            {
                vertices.push_back(b2Vec2(utility::toMeter(vertexIterator->FloatAttribute("x")),
                    utility::toMeter(vertexIterator->FloatAttribute("y"))));
            }
            // Construct the b2Shape
            std::unique_ptr<b2PolygonShape> ps(new b2PolygonShape);
            float area = computeArea(vertices);
            // If the area is negative, the polygon is either messed up or defined in wrong order.
            // A wrong order can be fixed by simply reversing all given vertices.
            if(area < 0)
                std::reverse(begin(vertices), end(vertices));
            ps->Set(vertices.data(), static_cast<int>(vertices.size()));
            physics.shapes.push_back(std::move(ps));
        }
    }
    else if(name == "circle") // Load circle
    {
        std::unique_ptr<b2CircleShape> cs(new b2CircleShape);
        cs->m_radius = utility::toMeter(shape.FloatAttribute("radius"));
        physics.shapes.push_back(std::move(cs));
    }
    // TODO: else exception?

    // Load fixtures
    if(auto fixtureXml = physic.FirstChildElement("fixture"))
    {
        physics.fixtureDef.density = fixtureXml->FloatAttribute("density");
        physics.fixtureDef.friction = fixtureXml->FloatAttribute("friction");
        physics.fixtureDef.restitution = fixtureXml->FloatAttribute("restitution");
    }
    // TODO: else exception?

    return physics;
}

PhysicsParser::Physics PhysicsParser::parseBody(const tinyxml2::XMLElement& xml) const
{
    Physics values;
    auto bodyXml = xml.FirstChildElement("body");
    if(bodyXml == nullptr)
        return values;

    if(std::string(bodyXml->Attribute("type")) == "static")
        values.bodyDef.type = b2_staticBody;
    else if(std::string(bodyXml->Attribute("type")) == "kinematic")
        values.bodyDef.type = b2_kinematicBody;
    else if(std::string(bodyXml->Attribute("type")) == "dynamic")
        values.bodyDef.type = b2_dynamicBody;
    values.bodyDef.angle = utility::toRadian(bodyXml->FloatAttribute("angle"));
    values.bodyDef.fixedRotation = bodyXml->BoolAttribute("fixedRotation");
    values.bodyDef.angularDamping = bodyXml->BoolAttribute("angularDamping");
    values.controllers = parseKinematics(*bodyXml);
    return values;
}

PhysicsParser::Physics::Controllers PhysicsParser::parseKinematics(const tinyxml2::XMLElement& xml) const
{
    Physics::Controllers controllers;
    auto kinematics = xml.FirstChildElement("kinematics");
    if(kinematics == nullptr)
        return controllers;

    auto rotation = kinematics->FirstChildElement("rotation");
    if(rotation != nullptr && rotation->FirstChildElement() != nullptr)
        controllers.rotation = m_providerParser.parseSingle(*rotation->FirstChildElement());

    ControllerParser parser(m_context);
    controllers.position[0] = parser.findController(*kinematics, "position", "axis", "x");
    controllers.position[1] = parser.findController(*kinematics, "position", "axis", "y");

    return controllers;
}
