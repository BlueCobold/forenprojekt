#pragma once

#ifndef JOINT_PARSER_HPP
#define JOINT_PARSER_HPP

#include "ProviderContext.hpp"

class Animation;
class AnimatedGraphics;
class CloneHandler;
class Entity;
class JointObject;
class ResourceManager;
class VariableHandler;

#include "tinyxml2.h"

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
    JointParser(ProviderContext context,
                ResourceManager& resourceManager,
                b2World& world,
                b2Body& body,
                unsigned int defaultTargetBuffer);

    std::vector<JointData> parse(const tinyxml2::XMLElement& xml);

private:
    JointData parseSingleRevoluteJoint(const tinyxml2::XMLElement& jointXml);
    JointData parseSinglePrismaticJoint(const tinyxml2::XMLElement& jointXml);
    JointData parseSingleDistanceJoint(const tinyxml2::XMLElement& jointXml);
    std::vector<std::unique_ptr<Animation>> parseAnimations(const tinyxml2::XMLElement& jointXml, JointObject& joint);
    void prepareAnimation(std::unique_ptr<Animation>& animation,
                          const tinyxml2::XMLElement& xml,
                          std::vector<std::unique_ptr<Animation>>& results,
                          JointObject& joint);

    ProviderContext m_context;
    ResourceManager& m_resourceManager;
    b2World& m_world;
    b2Body& m_body;
    unsigned int m_defaultTargetBuffer;
};

#endif // JOINT_PARSER_HPP
