#pragma once

namespace fw {

class Camera;
class Component;
class GameObject;
class Mesh;
class AABB;

class ComponentRegistry
{
public:
    ComponentRegistry();
    ComponentRegistry(const Component &) {};
    ComponentRegistry& operator=(const Component&) { return *this; }

    ~ComponentRegistry() { m_ComponentTypes.clear(); }

    typedef Component* (*CreateComponentFn)(void);
    std::map<const std::string, CreateComponentFn> m_ComponentTypes;

    void Register(const std::string &Name, CreateComponentFn pfnCreate);
    Component* GetComponent(const std::string& componentName);

    void AddInterfaceToCreateComponents(GameObject* pObject);
};

} // namespace fw
