#pragma once

#ifndef JOINT_PARSER_HPP
#define JOINT_PARSER_HPP

#include "AnimationContext.hpp"

class Animation;
class JointObject;

#include <tinyxml2.h>

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
    JointParser(AnimationContext& context,
                b2World& world,
                b2Body& body) :
        m_context(context),
        m_world(world),
        m_body(body)
    { }

    std::vector<JointData> parse(const tinyxml2::XMLElement& xml) const;

private:
    JointData parseSingleRevoluteJoint(const tinyxml2::XMLElement& jointXml) const;
    JointData parseSinglePrismaticJoint(const tinyxml2::XMLElement& jointXml) const;
    JointData parseSingleDistanceJoint(const tinyxml2::XMLElement& jointXml) const;
    std::vector<std::unique_ptr<Animation>> parseAnimations(const tinyxml2::XMLElement& jointXml, JointObject& joint) const;
    void prepareAnimation(Animation& animation,
                          const tinyxml2::XMLElement& xml,
                          JointObject& joint) const;

    AnimationContext& m_context;
    b2World& m_world;
    b2Body& m_body;
};

#endif // JOINT_PARSER_HPP
