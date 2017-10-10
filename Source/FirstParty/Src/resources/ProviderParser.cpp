
#include "ProviderParser.hpp"

#include "../animation/provider/Absolute.hpp"
#include "../animation/provider/Adder.hpp"
#include "../animation/provider/AngleProvider.hpp"
#include "../animation/provider/CachedProvider.hpp"
#include "../animation/provider/Clamp.hpp"
#include "../animation/provider/Count.hpp"
#include "../animation/provider/Delay.hpp"
#include "../animation/provider/FloatToInt.hpp"
#include "../animation/provider/IfPositive.hpp"
#include "../animation/provider/Inverse.hpp"
#include "../animation/provider/Maximum.hpp"
#include "../animation/provider/Minimum.hpp"
#include "../animation/provider/Modulo.hpp"
#include "../animation/provider/MouseProvider.hpp"
#include "../animation/provider/Multiplier.hpp"
#include "../animation/provider/Negate.hpp"
#include "../animation/provider/Nop.hpp"
#include "../animation/provider/Pow.hpp"
#include "../animation/provider/Ramp.hpp"
#include "../animation/provider/RandomProvider.hpp"
#include "../animation/provider/SetVariable.hpp"
#include "../animation/provider/Sine.hpp"
#include "../animation/provider/StaticProvider.hpp"
#include "../animation/provider/Step.hpp"
#include "../animation/provider/Stop.hpp"
#include "../animation/provider/Switch.hpp"
#include "../animation/provider/Substractor.hpp"
#include "../animation/provider/TimeProvider.hpp"
#include "../animation/provider/KeyProvider.hpp"
#include "../animation/provider/VariableProvider.hpp"

#include "../animation/CloneHandler.hpp"
#include "../animation/OrientedObject.hpp"
#include "../animation/TimedObject.hpp"
#include "../animation/VariableHandler.hpp"
#include "../animation/Stoppable.hpp"
#include "../Utility.hpp"

std::vector<std::unique_ptr<ValueProvider>> ProviderParser::parseMultiple(const tinyxml2::XMLElement& xml) const
{
    std::vector<std::unique_ptr<ValueProvider>> providers;
    for(auto child = xml.FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        if(std::string(child->Name()) == "function")
        {
            if(m_context.functions.size() == 0)
                throw std::runtime_error(utility::translateKey("@FunctionNull"));

            auto function = m_context.functions.find(child->Attribute("name"));
            if(function == end(m_context.functions))
                throw std::runtime_error(utility::replace(utility::translateKey("@NoTemplate"), child->Attribute("name")));

            auto subs = parseMultiple(*function->second);
            for(auto sub = begin(subs); sub != end(subs); ++sub)
                providers.push_back(std::move(*sub));
        }
        else
        {
            std::unique_ptr<ValueProvider> provider = parseSingle(*child);
            if(provider != nullptr)
                providers.push_back(std::move(provider));
        }
    }
    return providers;
}

template<typename T>
T* nonNull(T* param, std::string type)
{
    if(param == nullptr)
        throw std::runtime_error(std::string("A provider was used without source: ") + type);
    return param;
}

std::unique_ptr<ValueProvider> ProviderParser::parseSingle(const tinyxml2::XMLElement& xml) const
{
    std::string name(xml.Name());
    if(name == "time")
        return std::unique_ptr<TimeProvider>(new TimeProvider(*nonNull(m_context.timeSource, name),
                                                              m_context.cloneHandler.createTimeProviderCloneHandler()));
    else if(name == "stop")
        return std::unique_ptr<Stop>(new Stop(*nonNull(m_context.stoppable, name),
                                              m_context.cloneHandler.createStopProviderCloneHandler()));
    else if(name == "angle")
        return std::unique_ptr<AngleProvider>(new AngleProvider(*nonNull(m_context.angleSource, name),
                                                                m_context.cloneHandler.createAngleProviderCloneHandler()));
    else if(name == "static")
        return std::unique_ptr<StaticProvider>(new StaticProvider(xml.FloatAttribute("value")));
    else if(name == "var" || name == "variable")
        return std::unique_ptr<VariableProvider>(new VariableProvider(*nonNull(m_context.variableHandler, name), xml.Attribute("name"),
                                                                      m_context.cloneHandler.createVariableProviderCloneHandler()));
    else if(name == "count")
        return std::unique_ptr<Count>(new Count(xml.FloatAttribute("start"), xml.FloatAttribute("increment")));
    else if(name == "random")
        return std::unique_ptr<RandomProvider>(new RandomProvider(xml.FloatAttribute("min"), xml.FloatAttribute("max")));
    else if(name == "keyDown")
        return std::unique_ptr<KeyProvider>(new KeyProvider((int)(xml.Attribute("key")[0])));
    else if(name == "mouse")
        return std::unique_ptr<MouseProvider>(new MouseProvider(xml.Attribute("axis") ? std::string(xml.Attribute("axis")) == "x" : true));
    else if(name == "function")
    {
        if(m_context.functions.size() == 0)
            throw std::runtime_error(utility::translateKey("@FunctionNull"));

        auto funcName = xml.Attribute("name");
        if(funcName == nullptr)
            throw std::runtime_error(utility::translateKey("@NoName"));

        auto function = m_context.functions.find(funcName);
        if(function == end(m_context.functions))
            throw std::runtime_error(utility::replace(utility::translateKey("@NoTemplate"), funcName));

        auto providers = parseMultiple(*function->second);
        if(providers.size() > 1)
            throw std::runtime_error(utility::replace(utility::translateKey("@TemplateChild"), funcName));

        return std::move(providers[0]);
    }
    else
    {
        auto providers = parseMultiple(xml);
        if(name == "setVar" || name == "setVariable")
            return std::unique_ptr<SetVariable>(new SetVariable(*nonNull(m_context.variableHandler, name),
                                                                xml.Attribute("name"),
                                                                std::move(providers[0]),
                                                                xml.BoolAttribute("print"),
                                                                m_context.cloneHandler.createSetVariableProviderCloneHandler()));
        else if(name == "abs")
            return std::unique_ptr<Absolute>(new Absolute(std::move(providers[0])));
        else if(name == "sin" || name == "sine")
        {
            float amplitude = 1.f;
            float period = 1.f;
            xml.QueryFloatAttribute("amplitude", &amplitude);
            xml.QueryFloatAttribute("period", &period);
            return std::unique_ptr<Sine>(new Sine(std::move(providers[0]), amplitude, period));
        }
        else if(name == "cache")
            return std::unique_ptr<CachedProvider>(new CachedProvider(std::move(providers[0]),
                xml.Attribute("reset") ? std::string("true") != xml.Attribute("reset") : false));
        else if(name == "int" || name == "integer")
            return std::unique_ptr<FloatToInt>(new FloatToInt(std::move(providers[0])));
        else if(name == "add")
            return std::unique_ptr<Adder>(new Adder(std::move(providers)));
        else if(name == "ifpositive")
            return std::unique_ptr<IfPositive>(new IfPositive(std::move(providers)));
        else if(name == "mul")
            return std::unique_ptr<Multiplier>(new Multiplier(std::move(providers)));
        else if(name == "min")
            return std::unique_ptr<Minimum>(new Minimum(std::move(providers)));
        else if(name == "max")
            return std::unique_ptr<Maximum>(new Maximum(std::move(providers)));
        else if(name == "mod")
            return std::unique_ptr<Modulo>(new Modulo(std::move(providers)));
        else if(name == "pow")
            return std::unique_ptr<Pow>(new Pow(std::move(providers)));
        else if(name == "nop")
            return std::unique_ptr<Nop>(new Nop(std::move(providers)));
        else if(name == "clamp")
            return std::unique_ptr<Clamp>(new Clamp(std::move(providers)));
        else if(name == "step")
        {
            float threshold = 1;
            xml.QueryFloatAttribute("threshold", &threshold);
            return std::unique_ptr<Step>(new Step(std::move(providers), threshold));
        }
        else if(name == "sub")
            return std::unique_ptr<Substractor>(new Substractor(std::move(providers)));
        else if(name == "switch")
            return std::unique_ptr<Switch>(new Switch(std::move(providers), parseFloatList(xml.Attribute("cases"))));
        else if(name == "neg" || name == "negate")
            return std::unique_ptr<Negate>(new Negate(std::move(providers[0])));
        else if(name == "ramp")
            return std::unique_ptr<Ramp>(new Ramp(xml.FloatAttribute("min"), xml.FloatAttribute("max"),
                                                  std::move(providers[0])));
        else if(name == "delay")
            return std::unique_ptr<Delay>(new Delay(xml.FloatAttribute("start"), xml.FloatAttribute("duration"),
                                                    std::move(providers[0])));
        else if(name == "inv" || name == "invert")
            return std::unique_ptr<Inverse>(new Inverse(std::move(providers[0])));
        else if(m_context.customStopTags.size() > 0)
        {
            for(auto tag = begin(m_context.customStopTags); tag != end(m_context.customStopTags); ++tag)
            {
                if(name == tag->first)
                    return std::unique_ptr<Stop>(new Stop(*tag->second, m_context.cloneHandler.createStopProviderCloneHandler()));
            }
            throw std::runtime_error("Unregistered custom stop tag has been used on an invalid element");
        }
    }
    throw std::runtime_error(utility::replace(utility::translateKey("@Unknown"), name));
}

std::vector<double> ProviderParser::parseFloatList(const std::string& valueString) const
{
    std::vector<double> results;
    std::stringstream ss(valueString);
    std::string item;
    while(std::getline(ss, item, ','))
        results.push_back(utility::stringTo<double>(item));
    return results;
}
