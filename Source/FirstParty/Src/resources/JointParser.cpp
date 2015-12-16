
#include "JointParser.hpp"

#include "ResourceManager.hpp"
#include "LevelFileLoader.hpp"
#include "../animation/CloneHandler.hpp"
#include "../animation/VariableHandler.hpp"
#include "../model/AnimatedGraphics.hpp"
#include "../model/Entity.hpp"
#include "../model/joint/JointObject.hpp"
#include "../model/joint/SingleDistanceJoint.hpp"
#include "../model/joint/SinglePrismaticJoint.hpp"
#include "../model/joint/SingleRevoluteJoint.hpp"
#include "../model/JointValueProvider.hpp"

#include "Box2D/Dynamics/b2World.h"

#include "tinyxml2.h"

JointData::JointData(JointData&& other) :
    joint(std::move(other.joint)),
    animations(std::move(other.animations))
{ }

JointData& JointData::operator=(JointData&& other)
{
    joint = std::move(other.joint);
    animations = std::move(other.animations);
    return *this;
}

JointParser::JointParser(const tinyxml2::XMLElement& jointXml,
                         ResourceManager& resourceManager,
                         AnimatedGraphics& graphics,
                         VariableHandler& handler,
                         CloneHandler& cloneHandler,
                         b2World& world,
                         b2Body& body,
                         unsigned int defaultTargetBuffer) :
    m_xmlRoot(jointXml),
    m_resourceManager(resourceManager),
    m_graphics(graphics),
    m_handler(handler),
    m_cloneHandler(cloneHandler),
    m_world(world),
    m_body(body),
    m_defaultTargetBuffer(defaultTargetBuffer)
{ }

std::vector<JointData> JointParser::parse()
{
    std::vector<JointData> results;
    for(auto jointXml = m_xmlRoot.FirstChildElement("singleRevolute");
        jointXml != nullptr; jointXml = jointXml->NextSiblingElement("singleRevolute"))
    {
        results.emplace_back(parseSingleRevoluteJoint(*jointXml));
    }

    for(auto jointXml = m_xmlRoot.FirstChildElement("singlePrismatic");
        jointXml != nullptr; jointXml = jointXml->NextSiblingElement("singlePrismatic"))
    {
        results.emplace_back(parseSinglePrismaticJoint(*jointXml));
    }

    for(auto jointXml = m_xmlRoot.FirstChildElement("singleDistance");
        jointXml != nullptr; jointXml = jointXml->NextSiblingElement("singleDistance"))
    {
        results.emplace_back(parseSingleDistanceJoint(*jointXml));
    }
    return std::move(results);
}

JointData JointParser::parseSingleRevoluteJoint(const tinyxml2::XMLElement& jointXml)
{
    b2RevoluteJointDef jointDef;

    jointDef.localAnchorA = b2Vec2(utility::toMeter(jointXml.FloatAttribute("x")),
                                   utility::toMeter(jointXml.FloatAttribute("y")));

    //clockwise
    if(auto value = jointXml.FloatAttribute("cwlimit"))
        jointDef.lowerAngle = value / 360.f * b2_pi;

    // counter-clockwise
    if(auto value = jointXml.FloatAttribute("ccwlimit"))
        jointDef.upperAngle = value / 360.f * b2_pi;

    if(jointDef.upperAngle != 0 || jointDef.lowerAngle != 0)
        jointDef.enableLimit = true;

    // load motor data
    if(auto motor = jointXml.FirstChildElement("motor"))
    {
        jointDef.maxMotorTorque = motor->FloatAttribute("maxTorque");
        jointDef.motorSpeed = motor->FloatAttribute("speed");
        jointDef.enableMotor = true;
    }

    JointData result;
    result.joint = std::unique_ptr<SingleRevoluteJoint>(new SingleRevoluteJoint(m_world, jointDef, m_body));
    result.animations = parseAnimations(jointXml, *result.joint.get());
    return result;
}

JointData JointParser::parseSinglePrismaticJoint(const tinyxml2::XMLElement& jointXml)
{
    b2PrismaticJointDef jointDef;

    jointDef.localAnchorA = b2Vec2(utility::toMeter(jointXml.FloatAttribute("x")),
                                   utility::toMeter(jointXml.FloatAttribute("y")));

    // end point backward
    if(auto value = jointXml.FloatAttribute("endPointBackward"))
        jointDef.lowerTranslation = utility::toMeter(value);

    // end point forward
    if(auto value = jointXml.FloatAttribute("endPointForward"))
        jointDef.upperTranslation = utility::toMeter(value);

    if(jointDef.lowerTranslation != 0 || jointDef.upperTranslation != 0)
        jointDef.enableLimit = true;

    b2Vec2 direction(utility::toMeter(jointXml.FloatAttribute("directionX")),
                     utility::toMeter(jointXml.FloatAttribute("directionY")));

    // load motor data
    if(auto motor = jointXml.FirstChildElement("motor"))
    {
        jointDef.maxMotorForce = motor->FloatAttribute("maxForce");
        jointDef.motorSpeed = motor->FloatAttribute("speed");
        jointDef.enableMotor = true;
    }
    
    JointData result;
    result.joint = std::unique_ptr<SinglePrismaticJoint>(new SinglePrismaticJoint(m_world, jointDef, m_body, direction));
    result.animations = parseAnimations(jointXml, *result.joint.get());
    return result;
}

JointData JointParser::parseSingleDistanceJoint(const tinyxml2::XMLElement& jointXml)
{
    b2DistanceJointDef jointDef;

    jointDef.localAnchorA = b2Vec2(utility::toMeter(jointXml.FloatAttribute("x")),
                                   utility::toMeter(jointXml.FloatAttribute("y")));

    jointDef.localAnchorB = b2Vec2(utility::toMeter(jointXml.FloatAttribute("anchorX")),
                                   utility::toMeter(jointXml.FloatAttribute("anchorY")));

    if(auto value = jointXml.FloatAttribute("dampingRatio"))
        jointDef.dampingRatio = value;

    if(auto value = jointXml.FloatAttribute("frequencyHz"))
        jointDef.frequencyHz = value;
    
    JointData result;
    result.joint = std::unique_ptr<SingleDistanceJoint>(new SingleDistanceJoint(m_world, jointDef, m_body));
    result.animations = parseAnimations(jointXml, *result.joint.get());
    return result;
}

std::vector<std::unique_ptr<Animation>> JointParser::parseAnimations(const tinyxml2::XMLElement& jointXml, JointObject& joint)
{
    std::vector<std::unique_ptr<Animation>> results;
    static std::unordered_map<const Animation*, const Animation*> clonedAnimations;
    static std::unordered_map<const JointObject*, const JointObject*> clonedJoints;

    if(auto animXmls = jointXml.FirstChildElement("animations"))
    {
        for(auto animXml = animXmls->FirstChildElement("animation"); animXml != nullptr;
            animXml = animXml->NextSiblingElement("animation"))
        {
            if(auto animation = LevelFileLoader::parseAnimation(animXml,
                                                                &m_graphics,
                                                                &m_handler,
                                                                m_resourceManager,
                                                                nullptr,
                                                                m_cloneHandler))
            {
                if(animation->getBufferId() == UINT_MAX)
                    animation->setBufferId(m_defaultTargetBuffer);

                joint.registerCopyCallbacks(
                    [&](const JointObject& src, JointObject& clone) { clonedJoints.insert(std::make_pair(&src, &clone)); }, nullptr);

                animation->registerCloneCallbacks(
                    [&](const Animation& src, Animation& clone) { clonedAnimations.insert(std::make_pair(&src, &clone)); },
                    [&](const Animation& src, Animation& clone) { clonedAnimations.erase(clonedAnimations.find(&src)); });
                
                ProviderLocation index = ::Link;
                if(auto adherence = animXml->Attribute("location"))
                {
                    if(std::string(adherence) == "entity")
                        index = ::Entity;
                    else if(std::string(adherence) == "anchor")
                        index = ::Anchor;
                    else if(std::string(adherence) == "intermediate")
                        index = ::Intermediate;
                }

                if(index == ::Link || index == ::Intermediate)
                {
                    animation->bindScaleController(nullptr, std::unique_ptr<JointScaleProvider>(new JointScaleProvider(joint, *animation.get(),
                        [&](const JointObserver& old) -> const JointObject& { return *clonedJoints[&old.getObserved()]; },
                        [&](const AnimationObserver& old) -> const Animation& { return *clonedAnimations[&old.getObserved()]; }, index)));

                    animation->bindRotationController(std::unique_ptr<JointRotationProvider>(new JointRotationProvider(joint, 
                        [&](const JointObserver& old) -> const JointObject& { return *clonedJoints[&old.getObserved()]; }, index)));
                }
                
                animation->bindPositionController(
                    std::unique_ptr<JointPositionProvider>(new JointPositionProvider(joint, true, 
                    [&](const JointObserver& old) -> const JointObject& { return *clonedJoints[&old.getObserved()]; }, index)),
                    std::unique_ptr<JointPositionProvider>(new JointPositionProvider(joint, false, 
                    [&](const JointObserver& old) -> const JointObject&
                    {
                        auto& result = *clonedJoints[&old.getObserved()];
                        // This erase works on the very evil assumption of Animation::clone cloning the y-pos-provider as last one!
                        // This should actually be assured by some unit-test or something.
                        clonedJoints.erase(clonedJoints.find(&old.getObserved()));
                        return result;
                    }, index)));

                results.emplace_back(std::move(animation));
            }
        }
    }
    return results;
}
