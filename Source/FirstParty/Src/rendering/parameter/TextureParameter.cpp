
#include "TextureParameter.hpp"
#include "../GLExt.hpp"

TextureParameter::TextureParameter(const std::string& name, const sf::Texture& texture, int textureUnit) :
    ShaderParameter(name),
    m_texture(&texture),
    m_textureUnit(textureUnit)
{ }

void TextureParameter::bind()
{
#ifndef NO_SHADER
    auto location = getParamLocation();
    if(location < 0)
        return;

    gl::Uniform1i(location, m_textureUnit);
    if(m_textureUnit > 0)
    {
        gl::ActiveTexture(gl::TEXTURE0 + m_textureUnit);
        sf::Texture::bind(m_texture);
        gl::ActiveTexture(gl::TEXTURE0);
    }
#endif
}
