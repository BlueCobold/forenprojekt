
#include "SpriteSheet.hpp"
#include "../resources/BlendingParser.hpp"
#include "../Utility.hpp"

#include <tinyxml2.h>

SpriteSheet::SpriteSheet(const std::string& fileName) :
    m_textureName(""),
    m_fileName(fileName)
{
    loadFromFile(m_fileName);
}

SpriteSheet::SpriteSheet() :
    m_textureName(""),
    m_fileName("")
{
}

bool SpriteSheet::loadFromFile(const std::string& filename)
{
    m_fileName = filename;

    tinyxml2::XMLDocument doc;
    doc.LoadFile(m_fileName.c_str());
    if(doc.Error())
    {
        doc.PrintError();
        throw std::runtime_error(utility::replace(utility::translateKey("@MissingFile"), filename));
    }

    m_spriteKeys.clear();

    if(auto spritesheet = doc.FirstChildElement("spriteSheet"))
    {
        m_blendMode = parseBlending(*spritesheet);
        if(auto name = spritesheet->Attribute("texture"))
            m_textureName = name;
        else
            throw std::runtime_error(utility::replace(utility::translateKey("@InvalidXml"), filename));

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
                spriteData.originX = sprite->FloatAttribute("originx");
                spriteData.originY = sprite->FloatAttribute("originy");
                spriteData.offsetX = sprite->IntAttribute("xoffset");
                spriteData.offsetY = sprite->IntAttribute("yoffset");
                insert(key, spriteData);
            }
        }
        else
            throw std::runtime_error(utility::replace(utility::translateKey("@InvalidXml"), filename));
    }

    return true;
}

void SpriteSheet::insert(const std::string& key, const SpriteData& data)
{
    auto sprite = data;
    sprite.blendMode = m_blendMode;
    if(m_spriteKeys.find(key) == std::end(m_spriteKeys))
        m_spriteKeys[key] = sprite;
    else
        throw std::runtime_error(utility::replace(utility::translateKey("@DoubleListing"), key));
}

SpriteSheet::SpriteData SpriteSheet::get(const std::string& key) const
{
    auto result = m_spriteKeys.find(key);
    if(result == end(m_spriteKeys))
        throw std::runtime_error(utility::replace(utility::translateKey("@SpriteKeyNotFound"), key));
    return result->second;
}

std::string SpriteSheet::getTextureName() const
{
    return m_textureName;
}
