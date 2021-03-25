#pragma once

namespace fw {

class Component;
class CameraComponent;
class GameObject;
class Mesh;
class AABB;

class ComponentManager
{
public:
    ComponentManager();
    virtual ~ComponentManager();

    void Draw(CameraComponent* pCamera);
    void Update(float deltaTime);
    void AddToInspector();

#if FW_USING_LUA
    void UpdateLuaScriptComponents(float deltaTime);
#endif
    void AddComponent(Component* pComponent);
    void RemoveComponent(Component* pComponent);

    std::vector<Component*>& GetComponentList(const char* type);

protected:
    void DrawMeshComponents(CameraComponent* pCamera);
    void DrawDebugAABBComponents(CameraComponent* pCamera);
    void DrawParticleEmitterComponents(CameraComponent* pCamera);

    void UpdateParticleEmitterComponents(float deltaTime);

protected:
    std::map<const char*, std::vector<Component*>> m_Components;
};

} // namespace fw
