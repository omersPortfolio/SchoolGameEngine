#pragma once

#include "Math/Vector.h"
#include "Utility/Color.h"
#include "EventSystem/EventListener.h"
#include "Physics/PhysicsWorld.h"
#include "Material.h"
#include "Utility/JSONHelpers.h"

namespace fw {

class Camera;
class GameCore;
class Mesh;
class Scene;
class ShaderProgram;
class Texture;
class Component;
class Material;
class PhysicsBody;

class GameObject : public EventListener
{
public:
    GameObject(GameObject& orgnlGO);
    GameObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Material* pMaterial);
    virtual ~GameObject();

    virtual void Update(float deltaTime);
    virtual void Draw(Camera* pCamera);

    virtual void Save(WriterType& writer);

    virtual void ImGuiInspector();

    void AddComponent(Component* pComponent);
    void CreatePhysicsBodyCircle(BodyType bodyType, float radius, float density = 0.0f);
    void CreatePhysicsBodyBox(BodyType bodyType, float boxWidth, float boxHeight, float density = 0.0f);

    // Getters.
    std::string GetName() { return m_Name; }
    Scene* GetScene() { return m_pScene; }

    virtual const char* GetType() { return className.c_str(); }

    vec3 GetPosition() { return m_Position; }
    vec3 GetRotation() { return m_Rotation; }
    vec3 GetScale() { return m_Scale; }
    PhysicsBody* GetPhysicsBody() { return m_pPhysicsBody; }

    Material* GetMaterial() { return m_pMaterial; }

    // Setters.
    void SetName(std::string name) { m_Name = name; }
    void SetPosition(vec2 pos) { m_Position = pos; SetPositionPhysicsBody(); }
    void SetPosition(float x, float y) { m_Position.x = x; m_Position.y = y; SetPositionPhysicsBody(); }
    void SetRotation(vec2 rot) { m_Rotation = rot; /*SetRotationPhysicsBody*/ }
    void SetRotation(float x, float y) { m_Rotation.x = x; m_Rotation.y = y; /*SetRotationPhysicsBody*/ }
    //When using set position automatically move the physics body with the object
    void SetPositionPhysicsBody();

protected:
    Scene* m_pScene = nullptr;

    std::vector<Component*> m_Components;

    std::string m_Name;
    std::string className= "GameObject";//Used to identify object's tip(actual) class ,can be used when and if scripts are added as components or for duplication

    vec3 m_Position;
    vec3 m_Rotation;
    vec3 m_Scale;

    Material* m_pMaterial = nullptr;

    PhysicsBody* m_pPhysicsBody = nullptr;
};

} // namespace fw
