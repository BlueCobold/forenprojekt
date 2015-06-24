
#include "Shader.hpp"
#include "DrawParameter.hpp"
#include "GLExt.hpp"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) :
    m_programId(-1)
{
    if(!m_shader.loadFromFile(vertexPath, fragmentPath))
        throw std::runtime_error(utility::replace(utility::replace(utility::translateKey("ShaderFailed"), vertexPath), fragmentPath));
}

void Shader::addParameter(std::unique_ptr<ShaderParameter>&& parameter)
{
    m_parameters.push_back(std::move(parameter));
}

void Shader::prepare(const DrawParameter& param)
{
    for(auto it = begin(m_parameters); it != end(m_parameters); ++it)
        (*it)->prepare(param);

    param.activatePrimary();
    bind();
}

void Shader::bind()
{
#ifndef NO_SHADER
    sf::Shader::bind(&m_shader);
    if(m_programId < 0)
        gl::GetIntegerv(gl::CURRENT_PROGRAM, &m_programId);

    if(m_programId < 0)
        return;

    for(auto it = begin(m_parameters); it != end(m_parameters); ++it)
        (*it)->bind(m_programId);
#endif
}

void Shader::unbind()
{
#ifndef NO_SHADER
    sf::Shader::bind(nullptr);
#endif
}