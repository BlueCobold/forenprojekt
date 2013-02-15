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
    m_texture = *resourceManager.getTexture(bitmapfont->FirstChildElement("image")->Attribute("name"));
    height = bitmapfont->FirstChildElement("height")->IntAttribute("value"); 
    for(auto it = bitmapfont->FirstChildElement("glyphs")->FirstChildElement("glyph"); it != nullptr; it = it->NextSiblingElement("glyph"))
    {
        m_glyphs.insert
        ( 
            std::make_pair<char, sf::IntRect>(it->Attribute("name")[0], 
            sf::IntRect(it->IntAttribute("x"), it->IntAttribute("y"), it->IntAttribute("width"), height))
        );
    }

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

sf::Sprite BitmapFont::getSprite(const char key)
{
    if(m_glyphs.count(key))
        return sf::Sprite(m_texture, m_glyphs.at(key));

    return sf::Sprite();
}
