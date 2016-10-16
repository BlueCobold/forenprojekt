#pragma once

#ifndef TEXTURE_PARAMETER_HPP
#define TEXTURE_PARAMETER_HPP

#include "ShaderParameter.hpp"

#include <SFML/Graphics/Texture.hpp>

#include <string>

class TextureParameter : public ShaderParameter
{
public:
    TextureParameter(const std::string& name, const sf::Texture& texture, int textureUnit);

private:
    const sf::Texture* m_texture;
    const int m_textureUnit;
    
    virtual void onBind(int paramLocation) override;
    virtual void onPrepare(const DrawParameter& param) override;
};

#endif // TEXTURE_PARAMETER_HPP
