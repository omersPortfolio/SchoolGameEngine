#include "FrameworkPCH.h"

#include "PhysicsWorld.h"
#include "PhysicsBody.h"
#include "PhysicsWorld2D.h"
#include "PhysicsBody2D.h"
#include "box2d/box2d.h"
#include "Box2dDebugDraw.h"
#include "EventSystem/Event.h"
#include "EventSystem/EventManager.h"

namespace fw
{

class OurContactListener : public b2ContactListener
{
public:
    OurContactListener(EventManager* pEventManager)
    {
        m_pEventManager = pEventManager;
    }

    virtual void BeginContact(b2Contact* contact) override
    {
        b2Fixture* pFixtureA = contact->GetFixtureA();
        b2Fixture* pFixtureB = contact->GetFixtureB();
        
        b2Body* pBodyA = pFixtureA->GetBody();
        b2Body* pBodyB = pFixtureB->GetBody();

        PhysicsBody* pPhysicsBodyA = reinterpret_cast<PhysicsBody*>( pBodyA->GetUserData().pointer );
        PhysicsBody* pPhysicsBodyB = reinterpret_cast<PhysicsBody*>( pBodyB->GetUserData().pointer );

        // Send a collision event.
        PhysicsCollisionEvent* pCollisionEvent = new PhysicsCollisionEvent(pPhysicsBodyA, pPhysicsBodyB);
        m_pEventManager->AddEvent(pCollisionEvent);
    }

protected:
    EventManager* m_pEventManager = nullptr;
};

PhysicsWorld2D::PhysicsWorld2D(EventManager* pEventManager) :
    m_pWorld(nullptr)
{
    m_pWorld = new b2World(b2Vec2(0.0f, -10.0f));
    m_pWorld->SetContactListener( new OurContactListener(pEventManager) );

    m_pDebugDraw = new Box2dDebugDraw(nullptr);
    uint32 flags = b2Draw::e_shapeBit;
    flags |= b2Draw::e_jointBit;
    flags |= b2Draw::e_centerOfMassBit;
    flags |= b2Draw::e_aabbBit;
    flags |= b2Draw::e_pairBit;
    m_pDebugDraw->SetFlags(flags);
    m_pWorld->SetDebugDraw(m_pDebugDraw);
}

PhysicsWorld2D::~PhysicsWorld2D()
{
    delete m_pWorld->GetContactManager().m_contactListener;
    delete m_pDebugDraw;
    delete m_pWorld;
}

void PhysicsWorld2D::Update(float deltaTime)
{
    m_pWorld->Step(deltaTime, 8, 3);
}

PhysicsBody* PhysicsWorld2D::CreateBody(vec3 position, BodyType bodyType, bool bCanSleep)
{
    //Declare the bodyDef of default values for the b2Body.
    b2BodyDef bodyDef;

    //Sets the b2Body default position in the world space.
    bodyDef.position = b2Vec2(position.x, position.y);

    //Sets the b2Body type. Default is Static.
    switch(bodyType)
    {
    case StaticBody:
        bodyDef.type = b2_staticBody;
        break;

    case DynamicBody:
        bodyDef.type = b2_dynamicBody;
        break;
    };

    //Creates the b2Body in the world.
    b2Body* pBody = m_pWorld->CreateBody(&bodyDef);

    //Create PhysicsBody2D Wrapper around the b2Body.
    PhysicsBody2D* pBody2D = new PhysicsBody2D(pBody);

    //Return the new PhysicsBody to the world.
    return pBody2D;
}

void PhysicsWorld2D::DrawDebugData(vec4* view, vec4* proj)
{
    //m_pDebugDraw->SetViewAndProj(view, proj);
    m_pWorld->DebugDraw();
}

void PhysicsWorld2D::SetGravity(vec3 gravity)
{
    //Set the world's global gravity vector
    m_pWorld->SetGravity(b2Vec2(gravity.x, gravity.y));
}

void PhysicsWorld2D::SetCanSleep(bool bCanSleep)
{
    m_pWorld->SetAllowSleeping(bCanSleep);
}

}
