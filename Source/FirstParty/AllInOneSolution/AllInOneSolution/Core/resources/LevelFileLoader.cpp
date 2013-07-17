#include "LevelFileLoader.hpp"

#include "../animation/provider/Absolute.hpp"
#include "../animation/provider/Adder.hpp"
#include "../animation/provider/AngleProvider.hpp"
#include "../animation/provider/CachedProvider.hpp"
#include "../animation/provider/Clamp.hpp"
#include "../animation/provider/FloatToInt.hpp"
#include "../animation/provider/Inverse.hpp"
#include "../animation/provider/Maximum.hpp"
#include "../animation/provider/Minimum.hpp"
#include "../animation/provider/Modulo.hpp"
#include "../animation/provider/Multiplier.hpp"
#include "../animation/provider/Negate.hpp"
#include "../animation/provider/Nop.hpp"
#include "../animation/provider/Pow.hpp"
#include "../animation/provider/RandomProvider.hpp"
#include "../animation/provider/SetVariable.hpp"
#include "../animation/provider/Sine.hpp"
#include "../animation/provider/StaticProvider.hpp"
#include "../animation/provider/Step.hpp"
#include "../animation/provider/Stop.hpp"
#include "../animation/provider/Substractor.hpp"
#include "../animation/provider/TimeProvider.hpp"
#include "../animation/provider/VariableProvider.hpp"

#include <map>
#include <vector>

void LevelFileLoader::parseConstants(tinyxml2::XMLElement* xml,
    VariableHandler* holder)
{
    std::map<std::string, std::unique_ptr<ValueProvider>> map;
    for(auto var = xml->FirstAttribute(); var != nullptr; var = var->Next())
    {
        holder->setValueOf(var->Name(), var->FloatValue());
    }
}

std::vector<sf::Vector2i> parseValueList(tinyxml2::XMLElement* xml, const std::string& x, const std::string& y)
{
    std::vector<sf::Vector2i> result;
    auto xxml = xml->FirstChildElement(x.c_str());
    auto yxml = xml->FirstChildElement(y.c_str());
    if(!xxml || !yxml)
        return result;

    std::istringstream tokens(xxml->GetText());
    std::string token;
    while (std::getline(tokens, token, ',')) {
        result.push_back(sf::Vector2i(utility::stringTo<int>(token), 0));
    }
    tokens = std::istringstream(yxml->GetText());
    int i = 0;
    while (std::getline(tokens, token, ',')) {
        result[i].y = utility::stringTo<int>(token);
        ++i;
    }
    return result;
}

std::unique_ptr<Animation> LevelFileLoader::parseAnimation(tinyxml2::XMLElement* xml,
    AnimatedGraphics* animated,
    VariableHandler* handler,
    ResourceManager& resourceManager,
    std::unordered_map<std::string, tinyxml2::XMLElement*>* functions)
{
    tinyxml2::XMLElement* frameIndex = xml->FirstChildElement("frameindex");
    int frames = 1;
    if(frameIndex != nullptr)
        frames = frameIndex->IntAttribute("frames");
    
    sf::Texture* texture = resourceManager.getTexture(xml->Attribute("texture"));
    int width, height;
    if(xml->Attribute("width") != nullptr)
        width = xml->IntAttribute("width");
    else
        width = texture->getSize().x;
    if(xml->Attribute("height") != nullptr)
        height = xml->IntAttribute("height");
    else
        height = texture->getSize().y;

    bool rotate = xml->BoolAttribute("rotate");
    sf::Vector2f offset;
    offset.x = xml->FloatAttribute("x");
    offset.y = xml->FloatAttribute("y");
    sf::Vector2f origin;
    origin.x = xml->FloatAttribute("midx");
    origin.y = xml->FloatAttribute("midy");
    bool horizontal = true;
    if(xml->Attribute("alignment") != nullptr)
        horizontal = std::string("vertical") != xml->Attribute("alignment");
    
    std::unique_ptr<Animation> anim(new Animation(nullptr, frames, width, height, rotate, origin, offset, horizontal));
    
    std::unique_ptr<ValueProvider> provider;
    if(frameIndex == nullptr)
        provider.reset(new StaticProvider(0));
    else
    {
        tinyxml2::XMLElement* child = frameIndex->FirstChildElement();
        if(child == nullptr)
            throw std::runtime_error(utility::translateKey("NoProvider"));
        provider = std::move(parseProvider(child, animated, handler, anim.get(), functions));
    }
    anim->bindFrameProvider(std::move(provider));

    sf::Vector2f sourceOffset;
    sourceOffset.x = xml->FloatAttribute("srcx");
    sourceOffset.y = xml->FloatAttribute("srcy");

    anim->bindTexture(*texture, sourceOffset);

    std::vector<std::unique_ptr<Animation>> subAnimations;

    std::unique_ptr<ValueProvider> xProvider = findController(xml, animated, handler, anim.get(), "position", "axis", "x", functions);
    std::unique_ptr<ValueProvider> yProvider = findController(xml, animated, handler, anim.get(), "position", "axis", "y", functions);
    anim->bindPositionController(std::move(xProvider), std::move(yProvider));

    std::unique_ptr<ValueProvider> xScaleProvider = findController(xml, animated, handler, anim.get(), "scale", "axis", "x", functions);
    std::unique_ptr<ValueProvider> yScaleProvider = findController(xml, animated, handler, anim.get(), "scale", "axis", "y", functions);
    anim->bindScaleController(std::move(xScaleProvider), std::move(yScaleProvider));

    if(auto layout = xml->FirstChildElement("layout"))
    {
        std::vector<sf::Vector2i> srcoffsets = parseValueList(layout, "srcxoffset", "srcyoffset");
        std::vector<sf::Vector2i> sizes = parseValueList(layout, "width", "height");
        std::vector<sf::Vector2i> origins = parseValueList(layout, "midx", "midy");
        anim->setLayout(srcoffsets, sizes, origins);
    }

    std::unique_ptr<ValueProvider> red, green, blue, alpha, tmp;
    tinyxml2::XMLElement* static_color_element = xml->FirstChildElement("color");
    const char* value;
    while(static_color_element && !(value = static_color_element->Attribute("value")))
        static_color_element = static_color_element->NextSiblingElement("color");

    if(static_color_element && value)
    {
        sf::Color col = utility::hexToColor(value);
        red = std::unique_ptr<ValueProvider>(new StaticProvider(col.r / 255.f));
        green = std::unique_ptr<ValueProvider>(new StaticProvider(col.g / 255.f));
        blue = std::unique_ptr<ValueProvider>(new StaticProvider(col.b / 255.f));
        alpha = std::unique_ptr<ValueProvider>(new StaticProvider(col.a / 255.f));
    }

    tmp = findController(xml, animated, handler, anim.get(), "color", "channel", "red", functions);
    if(tmp != nullptr)
        red = std::move(tmp);
    tmp = findController(xml, animated, handler, anim.get(), "color", "channel", "green", functions);
    if(tmp != nullptr)
        green = std::move(tmp);
    tmp = findController(xml, animated, handler, anim.get(), "color", "channel", "blue", functions);
    if(tmp != nullptr)
        blue = std::move(tmp);
    tmp = findController(xml, animated, handler, anim.get(), "color", "channel", "alpha", functions);
    if(tmp != nullptr)
        alpha = std::move(tmp);

    anim->bindColorController(std::move(red), std::move(green), std::move(blue), std::move(alpha));

    if(auto blend = xml->Attribute("blending"))
    {
        sf::BlendMode mode = sf::BlendAlpha;
        if(std::string("add") == blend)
            mode = sf::BlendAdd;
        else if(std::string("mul") == blend)
            mode = sf::BlendMultiply;
        anim->setBlending(mode);
    }

    tinyxml2::XMLElement* rotation = xml->FirstChildElement("rotation");
    if(rotation != nullptr && rotation->FirstChildElement() != nullptr)
    {
        std::unique_ptr<ValueProvider> rotProvider = parseProvider(rotation->FirstChildElement(), animated, handler, anim.get(), functions);
        anim->bindRotationController(std::move(rotProvider));
    }

    tinyxml2::XMLElement* constants = xml->FirstChildElement("constants");
    if(constants != nullptr)
        parseConstants(constants, anim.get());

    return anim;
}

std::vector<std::unique_ptr<ValueProvider>> LevelFileLoader::parseProviders(tinyxml2::XMLElement* xml, 
    AnimatedObject* animated,
    VariableHandler* handler,
    Stoppable* stoppable,
    std::unordered_map<std::string, tinyxml2::XMLElement*>* functions)
{
    std::vector<std::unique_ptr<ValueProvider>> providers;
    for(auto child = xml->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        if(std::string(child->Name())=="function")
        {
            if(functions == nullptr)
                throw std::runtime_error(utility::translateKey("FunctionNull"));
            auto function = functions->find(child->Attribute("name"));
            if(function == end(*functions))
                throw std::runtime_error(utility::replace(utility::translateKey("NoTemplate"), child->Attribute("name")));
            return parseProviders(function->second, animated, handler, stoppable, functions);
        }
        std::unique_ptr<ValueProvider> provider = parseProvider(child, animated, handler, stoppable, functions);
        if(provider != nullptr)
            providers.push_back(std::move(provider));
    }
    return providers;
}

std::unique_ptr<ValueProvider> LevelFileLoader::parseProvider(tinyxml2::XMLElement* xml, 
    AnimatedObject* animated,
    VariableHandler* handler,
    Stoppable* stoppable,
    std::unordered_map<std::string, tinyxml2::XMLElement*>* functions)
{
    if(std::string(xml->Name())=="time")
        return std::unique_ptr<TimeProvider>(new TimeProvider(animated));
    else if(std::string(xml->Name())=="stop")
        return std::unique_ptr<Stop>(new Stop(animated));
    else if(std::string(xml->Name())=="stopAnimation")
    {
        if(stoppable != nullptr)
            return std::unique_ptr<Stop>(new Stop(stoppable));
        throw std::runtime_error("<stopAnimation> has been used on an invalid element");
    }
    else if(std::string(xml->Name())=="angle")
        return std::unique_ptr<AngleProvider>(new AngleProvider(animated));
    else if(std::string(xml->Name())=="static")
        return std::unique_ptr<StaticProvider>(new StaticProvider(xml->FloatAttribute("value")));
    else if(std::string(xml->Name())=="var")
        return std::unique_ptr<VariableProvider>(new VariableProvider(handler, xml->Attribute("name")));
    else if(std::string(xml->Name())=="setVar")
        return std::unique_ptr<SetVariable>(new SetVariable(handler, xml->Attribute("name"),
        std::move(parseProviders(xml, animated, handler, stoppable, functions)[0]), xml->BoolAttribute("print")));
    else if(std::string(xml->Name())=="abs")
        return std::unique_ptr<Absolute>(new Absolute(std::move(parseProviders(xml, animated, handler, stoppable, functions)[0])));
    else if(std::string(xml->Name())=="sine")
        return std::unique_ptr<Sine>(new Sine(std::move(parseProviders(xml, animated, handler, stoppable, functions)[0])));
    else if(std::string(xml->Name())=="cache")
        return std::unique_ptr<CachedProvider>(new CachedProvider(std::move(parseProviders(xml, animated, handler, stoppable, functions)[0]),
            xml->Attribute("reset") ? std::string("true") != xml->Attribute("reset") : false));
    else if(std::string(xml->Name())=="int")
        return std::unique_ptr<FloatToInt>(new FloatToInt(std::move(parseProviders(xml, animated, handler, stoppable, functions)[0])));
    else if(std::string(xml->Name())=="add")
        return std::unique_ptr<Adder>(new Adder(std::move(parseProviders(xml, animated, handler, stoppable, functions))));
    else if(std::string(xml->Name())=="mul")
        return std::unique_ptr<Multiplier>(new Multiplier(std::move(parseProviders(xml, animated, handler, stoppable, functions))));
    else if(std::string(xml->Name())=="min")
        return std::unique_ptr<Minimum>(new Minimum(std::move(parseProviders(xml, animated, handler, stoppable, functions))));
    else if(std::string(xml->Name())=="max")
        return std::unique_ptr<Maximum>(new Maximum(std::move(parseProviders(xml, animated, handler, stoppable, functions))));
    else if(std::string(xml->Name())=="mod")
        return std::unique_ptr<Modulo>(new Modulo(std::move(parseProviders(xml, animated, handler, stoppable, functions))));
    else if(std::string(xml->Name())=="pow")
        return std::unique_ptr<Pow>(new Pow(std::move(parseProviders(xml, animated, handler, stoppable, functions))));
    else if(std::string(xml->Name())=="nop")
        return std::unique_ptr<Nop>(new Nop(std::move(parseProviders(xml, animated, handler, stoppable, functions))));
    else if(std::string(xml->Name())=="clamp")
        return std::unique_ptr<Clamp>(new Clamp(std::move(parseProviders(xml, animated, handler, stoppable, functions))));
    else if(std::string(xml->Name())=="step")
        return std::unique_ptr<Step>(new Step(std::move(parseProviders(xml, animated, handler, stoppable, functions))));
    else if(std::string(xml->Name())=="sub")
        return std::unique_ptr<Substractor>(new Substractor(std::move(parseProviders(xml, animated, handler, stoppable, functions))));
    else if(std::string(xml->Name())=="neg")
        return std::unique_ptr<Negate>(new Negate(std::move(parseProviders(xml, animated, handler, stoppable, functions)[0])));
    else if(std::string(xml->Name())=="random")
        return std::unique_ptr<RandomProvider>(new RandomProvider(xml->FloatAttribute("min"), xml->FloatAttribute("max")));
    else if(std::string(xml->Name())=="inv")
        return std::unique_ptr<Inverse>(new Inverse(std::move(parseProviders(xml, animated, handler, stoppable, functions)[0])));
    else if(std::string(xml->Name())=="function")
    {
        if(functions == nullptr)
            throw std::runtime_error(utility::translateKey("FunctionNull"));
        auto funcName = xml->Attribute("name");
        if(funcName == nullptr)
            throw std::runtime_error(utility::translateKey("NoName"));
        auto function = functions->find(funcName);
        if(function == end(*functions))
            throw std::runtime_error(utility::replace(utility::translateKey("NoTemplate"), funcName));
        auto providers = parseProviders(function->second, animated, handler, stoppable, functions);
        if(providers.size()>1)
            throw std::runtime_error(utility::replace(utility::translateKey("TemplateChild"), funcName));
        return std::move(providers[0]);
    }
    else
        throw std::runtime_error(utility::replace(utility::translateKey("Unknown"), xml->Name()));
}

std::unordered_map<std::string, tinyxml2::XMLElement*> LevelFileLoader::parseList(
     tinyxml2::XMLElement* xml, const std::string& name, const std::string& key)
{
    std::unordered_map<std::string, tinyxml2::XMLElement*> tempList;

    for(auto iterator = xml->FirstChildElement(name.c_str());
        iterator != nullptr; iterator = iterator->NextSiblingElement(name.c_str()))
    {
        tempList.insert(std::make_pair<std::string, tinyxml2::XMLElement*>(
            std::string(iterator->Attribute(key.c_str())), iterator));
    }

    return std::move(tempList);
}

std::vector<std::string> LevelFileLoader::parseGrid(tinyxml2::XMLElement* xml)
{
    std::string data = xml->GetText();

    // Remove first and last line breaks
    data = data.substr(data.find_first_of('\n')+1, data.find_last_of('\n'));

    std::vector<std::string> lines;

    // Get the lines separated without the newline char
    for(std::size_t i = 0; i < data.length(); i = data.find('\n', i)+1)
        lines.push_back(data.substr(i, data.find('\n', i)-i));

    return std::move(lines);
}

void LevelFileLoader::parseKinematics(tinyxml2::XMLElement* element,
    Entity* entity,
    VariableHandler* handler,
    std::unordered_map<std::string, tinyxml2::XMLElement*>* functions)
{
    tinyxml2::XMLElement* kinematics = element->FirstChildElement("kinematics");
    if(kinematics == nullptr)
        return;
    tinyxml2::XMLElement* rotation = kinematics->FirstChildElement("rotation");
    if(rotation != nullptr && rotation->FirstChildElement() != nullptr)
    {
        std::unique_ptr<ValueProvider> provider = parseProvider(rotation->FirstChildElement(), entity, handler, nullptr, functions);
        if(provider != nullptr)
            entity->bindRotationController(std::move(provider));
    }
    std::unique_ptr<ValueProvider> posX = findController(kinematics, entity, handler, nullptr, "position", "axis", "x", functions);
    std::unique_ptr<ValueProvider> posY = findController(kinematics, entity, handler, nullptr, "position", "axis", "y", functions);
    if(posX != nullptr || posY != nullptr)
        entity->bindPositionController(std::move(posX), std::move(posY));
}

std::unique_ptr<ValueProvider> LevelFileLoader::findController(
    tinyxml2::XMLElement* xml,
    AnimatedGraphics* animated,
    VariableHandler* handler,
    Stoppable* stoppable,
    const std::string& childName,
    const std::string& propertyName,
    const std::string& propertyValue,
    std::unordered_map<std::string, tinyxml2::XMLElement*>* functions)
{
    for(auto iterator = xml->FirstChildElement(childName.c_str());
        iterator != nullptr; iterator = iterator->NextSiblingElement(childName.c_str()))
    {
        const char* value = iterator->Attribute(propertyName.c_str());
        if(value == nullptr || value != propertyValue)
            continue;
        if(iterator->FirstChildElement() == nullptr)
            throw std::runtime_error(utility::translateKey("SubTag"));
        return parseProvider(iterator->FirstChildElement(), animated, handler, stoppable, functions);
    }
    return nullptr;
}
