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
class ResourcesPanel;

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

    // Getters.
    std::string GetName() { return m_Name; }
    Scene* GetScene() { return m_pScene; }

    Component* GetFirstComponentOfType(const char* type);

    vec3 GetPosition() { return m_Position; }
    vec3 GetRotation() { return m_Rotation; }
    vec3 GetScale() { return m_Scale; }
    PhysicsBody* GetPhysicsBody() { return m_pPhysicsBody; }

    Material* GetMaterial() { return m_pMaterial; }

#if FW_USING_LUA
    static void LuaRegister(lua_State* luastate);
#endif

    // Setters.
    void SetName(std::string name) { m_Name = name; }

    void SetMaterial(Material* pMaterial) { m_pMaterial = pMaterial; }

    void SetScale(vec3 scale) { m_Scale = scale;}
    void SetPosition(vec3 pos) { m_Position = pos; }
    void SetRotation(vec3 rot) { m_Rotation = rot; }
    
protected:
    Scene* m_pScene = nullptr;

    std::vector<Component*> m_Components;

    std::string m_Name;

    vec3 m_Position;
    vec3 m_Rotation;
    vec3 m_Scale;

    Material* m_pMaterial = nullptr;

    PhysicsBody* m_pPhysicsBody = nullptr;
};

} // namespace fw
