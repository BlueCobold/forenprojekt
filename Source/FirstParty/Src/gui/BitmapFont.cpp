#include "BitmapFont.hpp"
#include "../resources/ResourceManager.hpp"

#include <SFML/System/Err.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <tinyxml2.h>

#include <utility>

BitmapFont::Glyph::Glyph(const sf::Texture& texture,
    const sf::IntRect& rectangle,
    int spacing,
    int xOffset,
    int yOffset) :
        Sprite(texture, rectangle),
        m_spacing(spacing),
        m_xOffset(xOffset),
        m_yOffset(yOffset)
{
}

int BitmapFont::Glyph::getSpacing() const
{
    return static_cast<int>(m_spacing * getScale().x);
}

int BitmapFont::Glyph::getVerticalOffset() const 
{
    return static_cast<int>(m_yOffset * getScale().y);
}

void BitmapFont::Glyph::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Sprite tmp = *this;
    tmp.move(static_cast<float>(m_xOffset), 0);
    target.draw(tmp, states);
}

bool BitmapFont::loadFromFile(const std::string& path, ResourceManager& resourceManager)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path.c_str());
    
    if(!validate(doc)) // Validate the XML file
    {
        sf::err() << "XML level file '" << path << "' is not valid!" << std::endl;
        return false;
    }
    m_fileName = path;

    auto bitmapfont = doc.FirstChildElement("bitmapfont");
    m_texture = resourceManager.getTexture(bitmapfont->FirstChildElement("image")->Attribute("name"));
    auto height = bitmapfont->FirstChildElement("height")->IntAttribute("value"); 
    for(auto it = bitmapfont->FirstChildElement("glyphs")->FirstChildElement("glyph"); it != nullptr; it = it->NextSiblingElement("glyph"))
    {
        int width = it->IntAttribute("width");
        int spacing = width;
        it->QueryIntAttribute("spacing", &spacing);

        BitmapFont::Glyph glyph = BitmapFont::Glyph(*m_texture,
            sf::IntRect(it->IntAttribute("x"), it->IntAttribute("y"), width, height),
            spacing, it->IntAttribute("xoffset"), it->IntAttribute("yoffset"));
        auto name = std::string(it->Attribute("name"));
        auto key = name[0];
        if(name.length() > 1 && name[0] == '#')
            key = utility::stringTo<int>(name.substr(1));
        m_glyphs.insert(std::make_pair(key, glyph));
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

    throw std::runtime_error(
        utility::replace(
            utility::replace(utility::translateKey("UnknownGlyph"), m_fileName),
        utility::toString(key)));
}

unsigned int BitmapFont::getFontSize()
{
    return m_fontSize;
}
