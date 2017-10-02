#include "figures.h"

Figures::Figures(uint id, QString name, DebugDraw *debugDraw) :
    B2Emitter(id, name),
    m_isEqualSize(false),
    m_isEqualColors(true),
    m_isCircles(false),
    m_isTriangles(false),
    m_isRectangles(true)
{
    m_id = id;
    m_name = name;
    this->g_debugDraw = *debugDraw;

    {
        b2BodyDef bd;
        bd.position.Set(0.0f, 0.0f);
        b2Body *body = m_world->CreateBody(&bd);

        b2EdgeShape shape;
        shape.Set(b2Vec2(50.0f, 0.0f), b2Vec2(-50.0f, 0.0f));

        body->CreateFixture(&shape, 0.0f);
    }

    m_size = FloatRange(0.250f, 0.800f);

    debugDraw->mainColor = QColor(Qt::cyan);
//    debugDraw->isEqualColors = false;
}

void Figures::Step(Settings *settings)
{
    B2Emitter::Step(settings);

    if (m_count < m_maxFigures) {

        b2Shape *shape;

        if (m_isCircles && !m_isTriangles && !m_isRectangles) {
            shape = CreateCircle();
        }
        else if (!m_isCircles && m_isTriangles && !m_isRectangles) {
            shape = CreateTriangle();
        }
        else if (!m_isCircles && !m_isTriangles && m_isRectangles) {
            shape = CreateRectangle();
        }
        else if (m_isCircles && m_isTriangles && !m_isRectangles) {
            int what = rand() % 2 + 1;
            shape = what == 1 ? CreateTriangle() : CreateCircle();
        }
        else if (m_isCircles && !m_isTriangles && m_isRectangles) {
            int what = rand() % 2 + 1;
            shape = what == 1 ? CreateRectangle() : CreateCircle();
        }
        else if (!m_isCircles && m_isTriangles && m_isRectangles) {
            int what = rand() % 2 + 1;
            shape = what == 1 ? CreateTriangle() : CreateRectangle();
        }
        else if (m_isCircles && m_isTriangles && m_isRectangles) {
            int what = rand() % 3 + 1;
            switch (what) {
            case 1: {
                shape = CreateTriangle();
                break;
            }
            case 2: {
                shape = CreateRectangle();
                break;
            }
            case 3: {
                shape = CreateCircle();
                break;
            }
            }
        }

        b2FixtureDef fd;
        fd.shape = shape;
        fd.density = 1.0f;
        fd.restitution = 0.4f;

        float32 x1 = g_debugDraw.g_camera->ConvertScreenToWorld(b2Vec2(0, 0)).x,
                x2 = g_debugDraw.g_camera->ConvertScreenToWorld(b2Vec2(g_debugDraw.g_camera->m_width, 0)).x;

        float32 y = g_debugDraw.g_camera->ConvertScreenToWorld(b2Vec2(0, 0)).y;

        float32 x = RandomFloat(x1, x2);
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.position.Set(x + 5.0f, y);
        bd.angle = RandomFloat(-b2_pi, b2_pi);
        bd.linearDamping = 0.0f;
        bd.angularDamping = 0.01f;
        b2Body *body = m_world->CreateBody(&bd);
        body->CreateFixture(&fd);
        m_count++;

    }
}

b2Shape *Figures::CreateTriangle()
{
    b2Transform xf1;
    xf1.q.Set(0.3524f * b2_pi);
    xf1.p = xf1.q.GetXAxis();

    b2Vec2 vertices[3];

    b2PolygonShape *triangle = new b2PolygonShape();

    if (m_isEqualSize) {
        float32 size = m_size.GetLeftBound();
        vertices[0] = b2Mul(xf1, b2Vec2(-1 * size, 0.0f));
        vertices[1] = b2Mul(xf1, b2Vec2(size, 0.0f));
        vertices[2] = b2Mul(xf1, b2Vec2(0.0f, size));
        triangle->Set(vertices, 3);
    }
    else {
        float32 size = RandomFloat(m_size.GetLeftBound(), m_size.GetRightBound());
        vertices[0] = b2Mul(xf1, b2Vec2(-1 * size, 0.0f));
        vertices[1] = b2Mul(xf1, b2Vec2(size, 0.0f));
        vertices[2] = b2Mul(xf1, b2Vec2(0.0f, size));
        triangle->Set(vertices, 3);
    }

    return triangle;
}

b2Shape *Figures::CreateCircle()
{
    b2CircleShape *circle = new b2CircleShape();

    if (m_isEqualSize) {
        float32 radius = m_size.GetLeftBound();
        circle->m_radius = radius;
        circle->m_p.Set(-1 * radius, radius);
    }
    else {
        float32 radius = RandomFloat(m_size.GetLeftBound(), m_size.GetRightBound());
        circle->m_radius = radius;
        circle->m_p.Set(-1 * radius, radius);
    }

    return circle;
}

b2Shape *Figures::CreateRectangle()
{
    b2PolygonShape *rectangle = new b2PolygonShape();
    if (m_isEqualSize) {
        float32 size = m_size.GetLeftBound();
        rectangle->SetAsBox(size, size, b2Vec2(-1 * size, 0.0f), 0.0f);
    }
    else {
        float32 size = RandomFloat(m_size.GetLeftBound(), m_size.GetRightBound());
        rectangle->SetAsBox(size, size, b2Vec2(-1 * size, 0.0f), 0.0f);
    }

    return rectangle;
}