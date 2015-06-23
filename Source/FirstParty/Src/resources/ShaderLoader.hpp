#pragma once

#ifndef SHADER_LOADER_HPP
#define SHADER_LOADER_HPP

#include "LevelFileLoader.hpp"
#include "../rendering/Shader.hpp"
#include "../MacHelper.hpp"
#include "../rendering/parameter/TextureParameter.hpp"
#include "../rendering/parameter/FloatParameter.hpp"

#include <SFML/Graphics/Shader.hpp>

#include <string>
#include <memory>

class ResourceManager;

class ShaderLoader
{
public:

    static Shader* loadShader(const std::string& vertexPath, 
                              const std::string& fragmentPath, 
                              const std::string& definitionPath,
                              ResourceManager& resourceManager)
    {
        if(!sf::Shader::isAvailable())
            return nullptr;

        auto shader = std::unique_ptr<Shader>(new Shader(resourcePath() + "res/shader/" + vertexPath,
                                                         resourcePath() + "res/shader/" + fragmentPath));
        auto sp = shader.get();

        loadConfig(shader.get(), definitionPath, resourceManager);

        shader.release();
        return sp;
    }

    static void loadConfig(Shader* shader, const std::string& path, ResourceManager& resourceManager)
    {
        tinyxml2::XMLDocument doc;
        doc.LoadFile((resourcePath() + "res/shader/" + path).c_str());

        if(doc.Error())
        {
            doc.PrintError();
            throw std::runtime_error(utility::replace(utility::translateKey("ShaderFileInvalid"), path));
        }
        tinyxml2::XMLElement* xml = doc.FirstChildElement("shader");
        if(xml == nullptr)
            throw std::runtime_error(utility::replace(utility::translateKey("ShaderEntryMissing"), path));

        parseParameters(shader, xml, resourceManager);
    }

    static void parseParameters(Shader* shader, tinyxml2::XMLElement* xml, ResourceManager& resourceManager)
    {
        if(auto params = xml->FirstChildElement("parameters"))
        {
            for(auto paramXml = params->FirstChildElement();
                paramXml != nullptr; paramXml = paramXml->NextSiblingElement())
            {
                std::string type = paramXml->Name();
                if(type == "sampler2D")
                {
                    if(paramXml->Attribute("uniform") && paramXml->Attribute("texture") && paramXml->Attribute("unit"))
                    {
                        auto param = std::unique_ptr<TextureParameter>(
                                        new TextureParameter(paramXml->Attribute("uniform"),
                                                             *resourceManager.getTexture(paramXml->Attribute("texture")),
                                                             paramXml->IntAttribute("unit")));
                        shader->addParameter(std::move(param));
                    }
                }
                else if (type == "float")
                {
                    if(auto child = paramXml->FirstChild() && paramXml->Attribute("uniform"))
                    {
                        static CloneHandler handler;
                        auto provider = LevelFileLoader::parseProvider(paramXml,
                                                                       &resourceManager.getShaderContext(),
                                                                       &resourceManager.getShaderContext(),
                                                                       nullptr,
                                                                       nullptr,
                                                                       handler);
                        auto param = std::unique_ptr<ShaderParameter>(
                                        new FloatParameter(paramXml->Attribute("uniform"), std::move(provider)));
                        shader->addParameter(std::move(param));
                    }
                }
            }
        }
    }
};

#endif //SHADER_LOADER_HPP
