#ifndef BITMAP_FONT_HPP
#define BITMAP_FONT_HPP

#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <map>

class ResourceManager;

namespace tinyxml2
{
    class XMLDocument;
}

class BitmapFont
{
public:

    bool loadFromFile(const std::string& path, ResourceManager& resourceManager);
    sf::Sprite getSprite(const char key);

private:
    bool validate(const tinyxml2::XMLDocument& document);

private:
    std::map<char, sf::IntRect> m_glyphs;
    sf::Texture m_texture;
};

#endif // BITMAP_FONT_HPP

