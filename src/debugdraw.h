#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include <Box2D/Box2D.h>
#include <src/rendertriangles.h>
#include <QPainter>

class DebugDraw : public b2Draw
{
public:
    DebugDraw();
    ~DebugDraw();

    void Create(Camera *camera, QOpenGLFunctions *functions);
    void Destroy();

    void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override {}

    void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override;

    void DrawCircle(const b2Vec2 &center, float32 radius, const b2Color &color) override {}

    void DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color) override;

    void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) override {}

    void DrawTransform(const b2Transform &xf) override {}

    void DrawPoint(const b2Vec2 &p, float32 size, const b2Color &color) override {}

    void Flush();

    QColor GetColor() const;

    void SetColor(const QColor &color);

private:
    RenderTriangles *m_triangles;

public:
    Camera *g_camera;
    QColor m_mainColor;
};

#endif // DEBUGDRAW_H
