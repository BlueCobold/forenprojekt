#include "BitmapFont.hpp"
#include "../resources/ResourceManager.hpp"

#include <SFML/System/Err.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <tinyxml2.h>

#include <utility>

BitmapFont::Glyph::Glyph(const sf::Texture& texture, int xOffset) : Sprite(texture), m_spacing(texture.getSize().x), m_xOffset(xOffset)
{
}

BitmapFont::Glyph::Glyph(const sf::Texture& texture, int spacing, int xOffset) : Sprite(texture), m_spacing(spacing), m_xOffset(xOffset)
{
}

BitmapFont::Glyph::Glyph(const sf::Texture& texture, const sf::IntRect& rectangle, int xOffset) : Sprite(texture, rectangle), m_spacing(rectangle.width), m_xOffset(xOffset)
{
}

BitmapFont::Glyph::Glyph(const sf::Texture& texture, const sf::IntRect& rectangle, int spacing, int xOffset) : Sprite(texture, rectangle), m_spacing(spacing), m_xOffset(xOffset)
{
}

int BitmapFont::Glyph::getSpacing() const
{
    return static_cast<int>(m_spacing * getScale().x);
}

void BitmapFont::Glyph::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Sprite tmp = *this;
    tmp.move(static_cast<float>(m_xOffset), 0);
    target.draw(tmp, states);
}

bool BitmapFont::loadFromFile(const std::string& path, ResourceManager& resourceManager)
{

    int height;
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path.c_str());
    
    if(!validate(doc)) // Validate the XML file
    {
        sf::err() << "XML level file '" << path << "' is not valid!" << std::endl;
        return false;
    }

    tinyxml2::XMLElement* bitmapfont = doc.FirstChildElement("bitmapfont");
    m_texture = resourceManager.getTexture(bitmapfont->FirstChildElement("image")->Attribute("name"));
    height = bitmapfont->FirstChildElement("height")->IntAttribute("value"); 
    for(auto it = bitmapfont->FirstChildElement("glyphs")->FirstChildElement("glyph"); it != nullptr; it = it->NextSiblingElement("glyph"))
    {
        int width = it->IntAttribute("width");
        int spacing = width;
        it->QueryIntAttribute("spacing", &spacing);

        BitmapFont::Glyph glyph = BitmapFont::Glyph(*m_texture, sf::IntRect(it->IntAttribute("x"), it->IntAttribute("y"), width, height), spacing, it->IntAttribute("xoffset"));
        m_glyphs.insert(std::make_pair(it->Attribute("name")[0], glyph));
    }
    m_fontSize = height;

    return true;
}

bool BitmapFont::validate(const tinyxml2::XMLDocument& document)
{
    if(document.Error())
    {
        document.PrintError();
        return false;
    }
        
    return true;
}

BitmapFont::Glyph BitmapFont::getGlyph(const char key) const
{
    if(m_glyphs.count(key))
        return m_glyphs.at(key);

    throw std::runtime_error(utility::replace(utility::translateKey("UnknownGlyph"), utility::toString(key)));
}

unsigned int BitmapFont::getFontSize()
{
    return m_fontSize;
}
