#ifndef BITMAP_FONT_HPP
#define BITMAP_FONT_HPP

#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <map>

class ResourceManager;

namespace tinyxml2
{
    class XMLDocument;
}

class BitmapFont : sf::NonCopyable
{
public:

    /// Class for a drawable character
    class Glyph : public sf::Sprite
    {
    public:

        Glyph(const sf::Texture& texture);
        Glyph(const sf::Texture& texture, int spacing);
        Glyph(const sf::Texture& texture, const sf::IntRect& rectangle);
        Glyph(const sf::Texture& texture, const sf::IntRect& rectangle, int spacing);

        int getSpacing() const;

    private:

        int m_spacing;
    };

    bool loadFromFile(const std::string& path, ResourceManager& resourceManager);
    Glyph getGlyph(const char key) const;

    unsigned int getFontSize();

private:
    bool validate(const tinyxml2::XMLDocument& document);

private:
    std::map<char, Glyph> m_glyphs;
    sf::Texture m_texture;
    unsigned int m_fontSize;
};

#endif // BITMAP_FONT_HPP

