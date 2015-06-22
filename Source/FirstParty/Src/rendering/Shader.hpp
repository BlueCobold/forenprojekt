#pragma once

#ifndef SHADER_HPP
#define SHADER_HPP

#include "parameter/ShaderParameter.hpp"

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

    void bind();
    void unbind();

private:
    sf::Shader m_shader;
    std::vector<std::unique_ptr<ShaderParameter>> m_parameters;
};

#endif // SHADER_HPP
