#include "SpriteSheet.hpp"
#include "../Utility.hpp"
#include <tinyxml2.h>

SpriteSheet::SpriteSheet(const std::string& fileName) :
    m_fileName(fileName),
    m_textureName("")
{
    loadFromFile(m_fileName);
}

SpriteSheet::SpriteSheet() :
    m_fileName(""),
    m_textureName("")
{
}

bool SpriteSheet::loadFromFile(const std::string& filename)
{
    m_fileName = filename;

    tinyxml2::XMLDocument doc;
    doc.LoadFile(filename.c_str());
    if(doc.Error())
    {
        doc.PrintError();
        throw std::runtime_error(utility::replace(utility::translateKey("MissingFile"), filename));
    }

    m_spriteKeys.clear();

    if(auto spritesheet = doc.FirstChildElement("spritesheet"))
    {
        m_textureName = spritesheet->Attribute("TextureName");
        if(auto sprites = spritesheet->FirstChildElement("sprites"))
        {
            for(auto sprite = sprites->FirstChildElement("sprite");
                sprite != nullptr; sprite = sprite->NextSiblingElement("sprite"))
            {
                SpriteData spriteData;
                std::string key = sprite->Attribute("name");
                spriteData.x = sprite->IntAttribute("x");
                spriteData.y = sprite->IntAttribute("y");
                spriteData.width = sprite->IntAttribute("width");
                spriteData.height = sprite->IntAttribute("height");
                spriteData.centerX = sprite->FloatAttribute("centerx");
                spriteData.centerY = sprite->FloatAttribute("centery");
                insert(key, spriteData);
            }
        }
    }

    return true;
}

void SpriteSheet::insert(const std::string& key, const SpriteData& data)
{
    if(m_spriteKeys.find(key) == std::end(m_spriteKeys))
        m_spriteKeys[key] = data;
    else
        throw std::runtime_error(utility::replace(utility::translateKey("doublelisting"), key));
}

std::string SpriteSheet::getTextureName() const
{
    return m_textureName;
}
