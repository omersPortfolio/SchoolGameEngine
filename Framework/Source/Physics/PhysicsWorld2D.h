#pragma once

class b2World;
class b2_draw;

namespace fw
{

class PhysicsBody2D;
class EventManager;

class PhysicsWorld2D : public PhysicsWorld
{
protected:
    b2World* m_pWorld;
    class Box2dDebugDraw* m_pDebugDraw;

public:
    PhysicsWorld2D(EventManager* pEventManager);
    virtual ~PhysicsWorld2D();

    virtual void Update(float deltaTime) override;

    virtual void DrawDebugData(vec4* view, vec4* proj);

    virtual PhysicsBody* CreateBody(vec3 position, BodyType bodyType, bool bCanSleep) override;

    virtual void SetGravity(vec3 gravity) override;
    virtual void SetCanSleep(bool bCanSleep) override;
};

}
