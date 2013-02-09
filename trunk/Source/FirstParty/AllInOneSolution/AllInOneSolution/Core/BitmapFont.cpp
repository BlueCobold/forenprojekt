#include "BitmapFont.hpp"
#include "resources/ResourceManager.hpp"

#include <SFML/System/Err.hpp>

#include <tinyxml2.h>

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
    height = bitmapfont->FirstChildElement("height")->IntAttribute("value");

    for(auto it = bitmapfont->FirstChildElement("glyphs"); it != nullptr; it = it->NextSiblingElement("glyphs"))
    {
        if(m_glyphs.count((char)it->Attribute("name") <= 0))
            m_glyphs.insert
            ( 
                std::make_pair<char, Glyph>((char)it->Attribute("name"), 
                Glyph( it->IntAttribute("x"), it->IntAttribute("y"), it->IntAttribute("width"), height))
            );
    }

    m_texture = *resourceManager.getTexture(bitmapfont->FirstChildElement("image")->Attribute("name"));

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