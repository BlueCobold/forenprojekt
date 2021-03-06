#include "DebugDraw.hpp"

#include "../Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <algorithm>

DebugDraw::DebugDraw(sf::RenderTarget& target) :
    m_target(target)
{
}


DebugDraw::~DebugDraw()
{
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    sf::ConvexShape polygon(vertexCount);    

    for(int i = 0 ; i < vertexCount; i++)
    {
        polygon.setPoint(i,
            sf::Vector2f(
            utility::toPixel(vertices[i].x), 
            utility::toPixel(vertices[i].y))
            );
    }

    polygon.setFillColor(sf::Color::Transparent);
    polygon.setOutlineColor(sf::Color::Red);
    polygon.setOutlineThickness(1.f);

    m_target.draw(polygon);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    DrawPolygon(vertices, vertexCount, color);
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
    sf::CircleShape circle(std::max(1.f, utility::toPixel(radius)));

    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::Red);

    circle.setPosition(
        utility::toPixel(center.x - radius),
        utility::toPixel(center.y - radius)
        );

   circle.setOutlineThickness(1.f);

    m_target.draw(circle);
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
    DrawCircle(center, radius, color);
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    sf::VertexArray lines(sf::Lines, 2);
    lines[0].color = sf::Color(0, 255, 0);
    lines[0].position = sf::Vector2f(utility::toPixel(p1.x), utility::toPixel(p1.y));
    lines[1].color = sf::Color(0, 255, 0);
    lines[1].position = sf::Vector2f(utility::toPixel(p2.x), utility::toPixel(p2.y));
    m_target.draw(lines);
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
}