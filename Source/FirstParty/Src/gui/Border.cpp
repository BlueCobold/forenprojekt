#include "Border.hpp"

#include "../Utility.hpp"
#include "../rendering/GLExt.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

Border::Border(int id,
               const ScreenLocation& position,
               const ScreenSize& size,
               const BorderStyle& style) :
    MenuElement(id, MenuElementType::Border, position),
    SizedElement(size),
    m_style(style),
    m_keepAspectRatio(false),
    m_scale(sf::Vector2f(1, 1)),
    m_hasDecos(m_style.decos[BorderStyle::Top].size() > 0
            || m_style.decos[BorderStyle::Right].size() > 0
            || m_style.decos[BorderStyle::Left].size() > 0
            || m_style.decos[BorderStyle::Bottom].size() > 0)
{ }

void Border::setScale(const sf::Vector2f& scale, bool keepAspectRatio)
{
    m_scale = scale;
    m_keepAspectRatio = keepAspectRatio;
}

void Border::updated(const sf::RenderWindow& screen, const double time, const sf::Vector2i& mouseOffset)
{
    auto screenSize = sf::Vector2f(screen.getSize());
    setScreenSize(screenSize);
    updateLayout(screenSize);
    auto size = getCurrentSize();
    size.x *= m_scale.x;
    size.y *= m_scale.y;

    size = sf::Vector2f(floorf(size.x + m_style.innerOffsets.width),
                        floorf(size.y + m_style.innerOffsets.height));

    auto tileHeight = m_style.backgrounds[BorderStyle::TopLeft].getTextureRect().height;
    auto tileWidth = m_style.backgrounds[BorderStyle::TopLeft].getTextureRect().width;

    auto pos = getCurrentPosition();
    auto position = sf::Vector2f(floorf(pos.x - size.x / 2.f + m_style.innerOffsets.left),
                                 floorf(pos.y - size.y / 2.f + m_style.innerOffsets.top));

    auto scalefactorHorizontal = size.x / m_style.backgrounds[BorderStyle::BottomCenter].getTextureRect().width;
    auto scalefactorVertical = size.y / m_style.backgrounds[BorderStyle::MiddleLeft].getTextureRect().height;

    m_style.backgrounds[BorderStyle::TopLeft].setPosition(position.x - tileWidth, position.y - tileHeight);
    m_style.backgrounds[BorderStyle::TopCenter].setPosition(position.x, position.y - tileHeight);
    m_style.backgrounds[BorderStyle::TopCenter].setScale(scalefactorHorizontal, 1.f);
    m_style.backgrounds[BorderStyle::TopRight].setPosition(position.x + size.x, position.y - tileHeight);
    m_style.backgrounds[BorderStyle::MiddleLeft].setPosition(position.x - tileWidth, position.y);
    m_style.backgrounds[BorderStyle::MiddleLeft].setScale(1.f, scalefactorVertical);
    m_style.backgrounds[BorderStyle::MiddleRight].setPosition(position.x + size.x, position.y);
    m_style.backgrounds[BorderStyle::MiddleRight].setScale(1.f, scalefactorVertical);
    m_style.backgrounds[BorderStyle::BottomLeft].setPosition(position.x - tileWidth, position.y + size.y);
    m_style.backgrounds[BorderStyle::BottomCenter].setPosition(position.x, position.y + size.y);
    m_style.backgrounds[BorderStyle::BottomCenter].setScale(scalefactorHorizontal, 1.f);
    m_style.backgrounds[BorderStyle::BottomRight].setPosition(position.x + size.x, position.y + size.y);

    auto it = m_style.backgrounds.find(BorderStyle::Background);
    if(it != end(m_style.backgrounds))
    {
        it->second.setPosition(position.x, position.y);
        it->second.setScale(size.x / it->second.getTextureRect().width,
                            size.y / it->second.getTextureRect().height);
    }

    updateDeco(BorderStyle::Top, position.x - tileWidth, position.y - tileHeight);
    updateDeco(BorderStyle::Right, position.x + size.x + tileWidth, position.y - tileHeight);
    updateDeco(BorderStyle::Left, position.x - tileWidth, position.y + size.y + tileHeight);
    updateDeco(BorderStyle::Bottom, position.x + size.x + tileWidth, position.y + size.y + tileHeight);
}

void Border::updateDeco(BorderStyle::DecoId id, float x, float y)
{
    auto& v = m_style.decos[id];
    for(auto it = begin(v); it != end(v); ++it)
        it->first.setPosition(x + it->second.x, y + it->second.y);
}

void Border::onDrawElement(const DrawParameter& params)
{
    if(m_hasDecos)
    {
        gl::DepthMask(true);
        gl::Enable(gl::DEPTH_TEST);
        gl::DepthFunc(gl::ALWAYS);
    }

    if(m_hasDecos)
        gl::DepthFunc(gl::LESS);

    for(auto it = begin(m_style.decos); it != end(m_style.decos); ++it)
        for(auto deco = begin(*it); deco != end(*it); ++deco)
        {
            deco->first.setColor(m_style.color);
            deco->first.draw(params);
        }

    for(auto it = begin(m_style.backgrounds); it != end(m_style.backgrounds); ++it)
    {
        it->second.setColor(m_style.color);
        it->second.draw(params);
    }

    if(m_hasDecos)
    {
        gl::Disable(gl::DEPTH_TEST);
        gl::DepthMask(false);
    }
}

std::unique_ptr<MenuElement> Border::onClone() const
{
    std::unique_ptr<Border> clone(new Border(getId(), getPosition(), getSize(), m_style));
    return std::move(clone);
}
