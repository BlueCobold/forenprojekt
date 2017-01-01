#include "Border.hpp"

#include "../Utility.hpp"
#include "../rendering/GLExt.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

Border::Border(int id,
               ScreenLocation position,
               ScreenSize size,
               std::unordered_map<BackgroundId, Sprite> backgrounds,
               const sf::FloatRect& innerOffsets,
               std::array<std::vector<std::pair<Sprite, sf::Vector2f>>, 4> decos) :
    MenuElement(id, MenuElementType::Border, position),
    m_backgrounds(std::move(backgrounds)),
    m_decos(std::move(decos)),
    m_size(size),
    m_hasDecos(m_decos[Top].size() > 0
            || m_decos[Right].size() > 0
            || m_decos[Left].size() > 0
            || m_decos[Bottom].size() > 0),
    m_keepAspectRatio(false),
    m_scale(sf::Vector2f(1, 1)),
    m_innerOffsets(innerOffsets)
{ }

void Border::setScale(const sf::Vector2f& scale, bool keepAspectRatio)
{
    m_scale = scale;
    m_keepAspectRatio = keepAspectRatio;
}

void Border::updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset)
{
    auto screenSize = sf::Vector2f(screen.getSize());
    m_size.setScreenSize(screenSize);
    updateLayout(screenSize);
    auto size = m_size.getCurrentSize();
    size.x *= m_scale.x;
    size.y *= m_scale.y;
    if(m_keepAspectRatio)
        size.y = size.x / m_size.getFixedAspectRatio();

    size = sf::Vector2f(floorf(size.x + m_innerOffsets.width),
                        floorf(size.y + m_innerOffsets.height));

    auto tileHeight = m_backgrounds[TopLeft].getTextureRect().height;
    auto tileWidth = m_backgrounds[TopLeft].getTextureRect().width;

    auto pos = getCurrentPosition();
    auto position = sf::Vector2f(floorf(pos.x - size.x / 2.f + m_innerOffsets.left),
                                 floorf(pos.y - size.y / 2.f + m_innerOffsets.top));

    auto scalefactorHorizontal = size.x / m_backgrounds[BottomCenter].getTextureRect().width;
    auto scalefactorVertical = size.y / m_backgrounds[MiddleLeft].getTextureRect().height;

    m_backgrounds[TopLeft].setPosition(position.x - tileWidth, position.y - tileHeight);
    m_backgrounds[TopCenter].setPosition(position.x, position.y - tileHeight);
    m_backgrounds[TopCenter].setScale(scalefactorHorizontal, 1.f);
    m_backgrounds[TopRight].setPosition(position.x + size.x, position.y - tileHeight);
    m_backgrounds[MiddleLeft].setPosition(position.x - tileWidth, position.y);
    m_backgrounds[MiddleLeft].setScale(1.f, scalefactorVertical);
    m_backgrounds[MiddleRight].setPosition(position.x + size.x, position.y);
    m_backgrounds[MiddleRight].setScale(1.f, scalefactorVertical);
    m_backgrounds[BottomLeft].setPosition(position.x - tileWidth, position.y + size.y);
    m_backgrounds[BottomCenter].setPosition(position.x, position.y + size.y);
    m_backgrounds[BottomCenter].setScale(scalefactorHorizontal, 1.f);
    m_backgrounds[BottomRight].setPosition(position.x + size.x, position.y + size.y);

    auto it = m_backgrounds.find(Background);
    if(it != end(m_backgrounds))
    {
        it->second.setPosition(position.x, position.y);
        it->second.setScale(size.x / it->second.getTextureRect().width,
                            size.y / it->second.getTextureRect().height);
    }

    updateDeco(Top, position.x - tileWidth, position.y - tileHeight);
    updateDeco(Right, position.x + size.x + tileWidth, position.y - tileHeight);
    updateDeco(Left, position.x - tileWidth, position.y + size.y + tileHeight);
    updateDeco(Bottom, position.x + size.x + tileWidth, position.y + size.y + tileHeight);
}

void Border::updateDeco(DecoId id, float x, float y)
{
    auto& v = m_decos[id];
    for(auto it = begin(v); it != end(v); ++it)
        it->first.setPosition(x + it->second.x, y + it->second.y);
}

void Border::doDraw(const DrawParameter& params)
{
    if(m_hasDecos)
    {
        gl::DepthMask(true);
        gl::Enable(gl::DEPTH_TEST);
        gl::DepthFunc(gl::ALWAYS);
    }

    for(auto it = begin(m_decos); it != end(m_decos); ++it)
        for(auto deco = begin(*it); deco != end(*it); ++deco)
            deco->first.draw(params);

    if(m_hasDecos)
        gl::DepthFunc(gl::LESS);

    for(auto it = begin(m_backgrounds); it != end(m_backgrounds); ++it)
        it->second.draw(params);

    if(m_hasDecos)
    {
        gl::Disable(gl::DEPTH_TEST);
        gl::DepthMask(false);
    }
}

std::unique_ptr<MenuElement> Border::doClone() const
{
    std::unique_ptr<Border> clone(new Border(getId(), ScreenLocation(getPosition(), getOffset()), m_size, m_backgrounds, m_innerOffsets, m_decos));
    clone->m_keepAspectRatio = m_keepAspectRatio;
    clone->setScale(m_scale, m_keepAspectRatio);
    return std::move(clone);
}
