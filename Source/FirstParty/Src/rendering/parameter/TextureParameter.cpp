
#include "TextureParameter.hpp"
#include "../DrawParameter.hpp"
#include "../GLExt.hpp"

TextureParameter::TextureParameter(const std::string& name, const sf::Texture& texture, int textureUnit) :
    ShaderParameter(name),
    m_texture(&texture),
    m_textureUnit(textureUnit)
{ }

void TextureParameter::onBind(int paramLocation)
{
#ifndef NO_SHADER
    gl::Uniform1i(paramLocation, m_textureUnit);
    if(m_textureUnit > 0)
    {
        gl::ActiveTexture(gl::TEXTURE0 + m_textureUnit);
        sf::Texture::bind(m_texture);
        gl::ActiveTexture(gl::TEXTURE0);
    }
#endif
}

void TextureParameter::onPrepare(const DrawParameter& param)
{
    param.prepareTexture(m_texture);
}
