#pragma once

#include <Box2D/Common/b2Draw.h>

namespace sf
{
    class RenderTarget;
}

/// This class will be used to implement debug drawing for box2d
class DebugDraw : public b2Draw
{
public:

    DebugDraw(sf::RenderTarget& target);
    ~DebugDraw();

    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
    void DrawTransform(const b2Transform& xf);

private:

    sf::RenderTarget& m_target;
};

