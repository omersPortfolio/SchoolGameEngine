#pragma once

#include "Math/Vector.h"

namespace fw
{

class PhysicsBody;

enum BodyType
{
    StaticBody = 0,
    DynamicBody
};

class PhysicsWorld
{
public:
    virtual ~PhysicsWorld() = 0 {}

    virtual void Update(float deltaTime) = 0;

    virtual PhysicsBody* CreateBody(vec3 position, BodyType bodyType, bool bCanSleep) = 0;

    virtual void SetGravity(vec3 gravity) = 0;
    virtual void SetCanSleep(bool bCanSleep) = 0;
};

}
