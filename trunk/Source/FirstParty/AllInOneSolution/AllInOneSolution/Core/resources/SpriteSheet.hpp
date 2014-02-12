#pragma once

#ifndef SPRITESHEET_HPP
#define SPRITESHEET_HPP

#include <unordered_map>
#include <iostream>

namespace tinyxml2
{
    class XMLDocument;
}

class SpriteSheet
{
public:
    struct SpriteData {int x;
                       int y;
                       int width;
                       int height;
                       float originX;
                       float originY;};
private:
    std::unordered_map<std::string, SpriteData> m_spriteKeys;
    std::string m_textureName;
    std::string m_fileName;

    void insert(const std::string& key, const SpriteData& data);

public:
    SpriteSheet(const std::string& fileName);
    SpriteSheet();
    SpriteData get(const std::string& key) const;
    std::string getTextureName() const;
    bool loadFromFile(const std::string& fileName);
};

#endif