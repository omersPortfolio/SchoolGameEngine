#pragma once

#include "Component.h"

namespace fw {

class Mesh;

class MeshComponent : public Component
{
public:
    MeshComponent() {}
    MeshComponent(Mesh* pMesh) { m_pMesh = pMesh; }

    static const char* GetStaticType() { return "MeshComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }
    static Component* Create() { return new MeshComponent(); }

    virtual void Save(WriterType& writer) override;
    virtual void Load(rapidjson::Value& component) override;

    // Getters.
    Mesh* GetMesh() { return m_pMesh; }

    // Setters.
    void SetMesh(Mesh* pMesh) { m_pMesh = pMesh; }

protected:
    Mesh* m_pMesh = nullptr;
};

} // namespace fw
