#ifndef TEST_H
#define TEST_H

#include "../debugdraw.h"
#include "emitter.h"
#include <Box2D/Box2D.h>

class B2Emitter;
struct Settings;

typedef B2Emitter *TestCreateFcn();

#define RAND_LIMIT  32767
#define DRAW_STRING_NEW_LINE 16

/// Random number in range [-1,1]
inline float32 RandomFloat()
{
    float32 r = (float32)(rand() & (RAND_LIMIT));
    r /= RAND_LIMIT;
    r = 2.0f * r - 1.0f;
    return r;
}

/// Random floating point number in range [lo, hi]
inline float32 RandomFloat(float32 lo, float32 hi)
{
    float32 r = (float32)(rand() & (RAND_LIMIT));
    r /= RAND_LIMIT;
    r = (hi - lo) * r + lo;
    return r;
}

/// Test settings. Some can be controlled in the GUI.
struct Settings {
    Settings()
    {
        hz = 60.0f;
        velocityIterations = 8;
        positionIterations = 3;
        drawShapes = true;
        drawJoints = true;
        drawAABBs = false;
        drawContactPoints = false;
        drawContactNormals = false;
        drawContactImpulse = false;
        drawFrictionImpulse = false;
        drawCOMs = false;
        drawStats = false;
        drawProfile = false;
        enableWarmStarting = true;
        enableContinuous = true;
        enableSubStepping = false;
        enableSleep = true;
        pause = false;
        singleStep = false;
    }

    float32 hz;
    int32 velocityIterations;
    int32 positionIterations;
    bool drawShapes;
    bool drawJoints;
    bool drawAABBs;
    bool drawContactPoints;
    bool drawContactNormals;
    bool drawContactImpulse;
    bool drawFrictionImpulse;
    bool drawCOMs;
    bool drawStats;
    bool drawProfile;
    bool enableWarmStarting;
    bool enableContinuous;
    bool enableSubStepping;
    bool enableSleep;
    bool pause;
    bool singleStep;
};

struct TestEntry {
    uint id;
    const char *name;
    TestCreateFcn *createFcn;
};

// This is called when a joint in the world is implicitly destroyed
// because an attached body is destroyed. This gives us a chance to
// nullify the mouse joint.
class DestructionListener : public b2DestructionListener
{
public:
    void SayGoodbye(b2Fixture *fixture) override
    {
        B2_NOT_USED(fixture);
    }
    void SayGoodbye(b2Joint *joint) override;

    B2Emitter *test;
};

const int32 k_maxContactPoints = 2048;

struct ContactPoint {
    b2Fixture *fixtureA;
    b2Fixture *fixtureB;
    b2Vec2 normal;
    b2Vec2 position;
    b2PointState state;
    float32 normalImpulse;
    float32 tangentImpulse;
    float32 separation;
};

class B2Emitter : public b2ContactListener, public Emitter
{
public:

    B2Emitter();
    B2Emitter(uint id, QString name);
    virtual ~B2Emitter();

    B2Emitter &operator=(const B2Emitter &from);

    void DrawTitle(const char *string);
    virtual void Step(Settings *settings);
    virtual void Keyboard(int key)
    {
        B2_NOT_USED(key);
    }
    virtual void KeyboardUp(int key)
    {
        B2_NOT_USED(key);
    }
    void ShiftMouseDown(const b2Vec2 &p);
    virtual void MouseDown(const b2Vec2 &p);
    virtual void MouseUp(const b2Vec2 &p);
    void MouseMove(const b2Vec2 &p);
    void LaunchBomb();
    void LaunchBomb(const b2Vec2 &position, const b2Vec2 &velocity);

    void SpawnBomb(const b2Vec2 &worldPt);
    void CompleteBombSpawn(const b2Vec2 &p);

    // Let derived tests know that a joint was destroyed.
    virtual void JointDestroyed(b2Joint *joint)
    {
        B2_NOT_USED(joint);
    }

    // Callbacks for derived classes.
    virtual void BeginContact(b2Contact *contact)  override
    {
        B2_NOT_USED(contact);
    }
    virtual void EndContact(b2Contact *contact)  override
    {
        B2_NOT_USED(contact);
    }
    virtual void PreSolve(b2Contact *contact, const b2Manifold *oldManifold) override;
    virtual void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) override
    {
        B2_NOT_USED(contact);
        B2_NOT_USED(impulse);
    }

    void ShiftOrigin(const b2Vec2 &newOrigin);

    EmitterType GetType()
    {
        return EmitterType::B2;
    }

    QString GetEmitterName()
    {
        return m_name;
    }

    void SetParticlesSpeed(uint value) {  }

    uint GetParticlesSpeed() {  }

    void SetParticlesSize(uint value) {  }

    uint GetParticlesSize() {  }

    void SetParticlesSaturation(uint value) {  }

    uint GetParticlesSaturation() {  }

    void SetId(uint id)
    {
        this->m_id = id;
    }

    void SetName(QString name)
    {
        m_name = name;
    }

protected:
    friend class DestructionListener;
    friend class BoundaryListener;
    friend class ContactListener;

    b2Body *m_groundBody;
    b2AABB m_worldAABB;
    ContactPoint m_points[k_maxContactPoints];
    int32 m_pointCount;
    DestructionListener m_destructionListener;
    int32 m_textLine;
    b2World *m_world;
    b2Body *m_bomb;
    b2MouseJoint *m_mouseJoint;
    b2Vec2 m_bombSpawnPoint;
    bool m_bombSpawning;
    b2Vec2 m_mouseWorld;
    int32 m_stepCount;

    b2Profile m_maxProfile;
    b2Profile m_totalProfile;

    QString m_name;
public:
    DebugDraw g_debugDraw;
    QPainter *m_painter;
};

#endif // TEST_H
