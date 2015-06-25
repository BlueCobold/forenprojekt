#pragma once

#ifndef SHADER_HPP
#define SHADER_HPP

#include "parameter/ShaderParameter.hpp"
class DrawParameter;

#include <SFML/Graphics/Shader.hpp>

#include <memory>
#include <string>
#include <vector>

#include "../Utility.hpp"

class Shader
{
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    void addParameter(std::unique_ptr<ShaderParameter>&& parameter);

    void prepare(const DrawParameter& param);
    void bind();
    void unbind();

    static bool isAvailable();
    static bool isUsable();
    static void allowUsage(bool allow);

private:
    static bool _shadersAllowed;
    sf::Shader m_shader;
    std::vector<std::unique_ptr<ShaderParameter>> m_parameters;
    int m_programId;
};

#endif // SHADER_HPP
