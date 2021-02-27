#pragma once

class b2World;
class b2Body;

namespace fw
{

class PhysicsBody2D : public PhysicsBody
{
protected:
    b2Body* m_pBody;

public:
    PhysicsBody2D(b2Body* body);
    virtual ~PhysicsBody2D();

    virtual void CreateCircle(float radius) override;
    virtual void CreateBox(float width, float height) override;

    virtual void SetPosition(vec3 position) override;
    virtual void SetAngle(float angle) override;
    virtual void SetBodyType(BodyType bodyType) override;
    virtual void SetCanSleep(bool bCanSleep) override;
    virtual void SetDensity(float density) override;

    virtual vec3 GetPosition() override;

    virtual void ApplyForceToCenter(vec3 force) override;
    virtual void ApplyImpulseToCenter(vec3 impulse) override;
};

}
