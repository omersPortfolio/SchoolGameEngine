#pragma once

namespace fw {

class Camera;
class Component;
class GameObject;
class Mesh;
class AABB;

class ComponentManager
{
public:
    ComponentManager();
    virtual ~ComponentManager();

    void Draw(Camera* pCamera);
    void Update(float deltaTime);

    void AddComponent(Component* pComponent);
    void RemoveComponent(Component* pComponent);

    std::vector<Component*>& GetComponentList(const char* type);

protected:
    void DrawMeshComponents(Camera* pCamera);
    void DrawDebugAABBComponents(Camera* pCamera);

protected:
    std::map<const char*, std::vector<Component*>> m_Components;
};

} // namespace fw
