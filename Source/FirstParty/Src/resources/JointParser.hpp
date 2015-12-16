#pragma once

#ifndef JOINT_PARSER_HPP
#define JOINT_PARSER_HPP

#include "tinyxml2.h"

class Animation;
class AnimatedGraphics;
class CloneHandler;
class Entity;
class JointObject;
class ResourceManager;
class VariableHandler;

class b2Body;
class b2World;

#include <memory>
#include <vector>

struct JointData
{
    std::unique_ptr<JointObject> joint;
    std::vector<std::unique_ptr<Animation>> animations;

    JointData()
    { }

    JointData(JointData&& other);
    JointData& operator=(JointData&& other);
};

class JointParser
{
public:
    JointParser(const tinyxml2::XMLElement& jointXml,
                ResourceManager& resourceManager,
                AnimatedGraphics& graphics,
                VariableHandler& handler,
                CloneHandler& cloneHandler,
                b2World& world,
                b2Body& body,
                unsigned int defaultTargetBuffer);

    std::vector<JointData> parse();

private:
    JointData parseSingleRevoluteJoint(const tinyxml2::XMLElement& jointXml);
    JointData parseSinglePrismaticJoint(const tinyxml2::XMLElement& jointXml);
    JointData parseSingleDistanceJoint(const tinyxml2::XMLElement& jointXml);
    std::vector<std::unique_ptr<Animation>> parseAnimations(const tinyxml2::XMLElement& jointXml, JointObject& joint);

    const tinyxml2::XMLElement& m_xmlRoot;
    ResourceManager& m_resourceManager;
    AnimatedGraphics& m_graphics;
    VariableHandler& m_handler;
    CloneHandler& m_cloneHandler;
    b2World& m_world;
    b2Body& m_body;
    unsigned int m_defaultTargetBuffer;
};

#endif // JOINT_PARSER_HPP
