#pragma once

#include "Component.h"

namespace fw {

class Mesh;
class Material;

class MeshComponent : public Component
{
public:
    MeshComponent() {}
    MeshComponent(Mesh* pMesh, Material* pMaterial) { m_pMesh = pMesh; m_pMaterial = pMaterial; }

    static const char* GetStaticType() { return "MeshComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }
    static Component* Create() { return new MeshComponent(); }

    virtual void Save(WriterType& writer) override;
    virtual void Load(rapidjson::Value& component) override;
    virtual void AddToInspector() override;

    virtual void Init() override;

    // Getters.
    Mesh* GetMesh() { return m_pMesh; }
    Material* GetMaterial() { return m_pMaterial; }

    // Setters.
    void SetMesh(Mesh* pMesh) { m_pMesh = pMesh; }
    void SetMaterial(Material* pMaterial) { m_pMaterial = pMaterial; }

protected:
    Mesh* m_pMesh = nullptr;
    Material* m_pMaterial = nullptr;
};

} // namespace fw
