
#include "Shader.hpp"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    if(!m_shader.loadFromFile(vertexPath, fragmentPath))
        throw std::runtime_error(utility::replace(utility::replace(utility::translateKey("ShaderFailed"), vertexPath), fragmentPath));
}

void Shader::addParameter(std::unique_ptr<ShaderParameter>&& parameter)
{
    m_parameters.push_back(std::move(parameter));
}

void Shader::bind()
{
#ifndef NO_SHADER
    sf::Shader::bind(&m_shader);
    for(auto it = begin(m_parameters); it != end(m_parameters); ++it)
        (*it)->bind();
#endif
}

void Shader::unbind()
{
#ifndef NO_SHADER
    sf::Shader::bind(nullptr);
#endif
}