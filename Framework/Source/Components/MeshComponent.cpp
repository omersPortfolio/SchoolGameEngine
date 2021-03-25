#include "FrameworkPCH.h"

#include "MeshComponent.h"
#include "Core/GameCore.h"
#include "Objects/GameObject.h"
#include "Objects/Mesh.h"
#include "Objects/ResourceManager.h"
#include "UI/ResourcesPanel.h"
#include "Scene/Scene.h"

namespace fw {

void MeshComponent::Save(WriterType& writer)
{
    Component::Save(writer);

    ResourceManager* pResources = m_pGameObject->GetScene()->GetGameCore()->GetResourceManager();

    JSONSaveCharArray(writer, "Mesh", pResources->FindMeshName(m_pMesh).c_str());
    JSONSaveCharArray(writer, "Material", pResources->FindMaterialName(m_pMaterial).c_str());

    //writer.Key("Verts");
    //writer.StartArray();
    //{
    //    if (m_pMesh != nullptr)
    //    {
    //        for (int i = 0; i < m_pMesh->GetVerts().size(); i++)
    //        {
    //            VertexFormat vertex = m_pMesh->GetVerts().at(i);
    //            writer.StartArray();
    //            {
    //                writer.Double(vertex.x);
    //                writer.Double(vertex.y);
    //                writer.Double(vertex.z);
    //                writer.Double(vertex.u);
    //                writer.Double(vertex.v);
    //                writer.EndArray();
    //            }
    //        }
    //    }
    //    writer.EndArray();
    //}
}

void MeshComponent::Load(rapidjson::Value& component)
{
    ResourceManager* pResources = m_pGameObject->GetScene()->GetGameCore()->GetResourceManager();

    if (component.HasMember("Mesh"))
    {
        m_pMesh = pResources->GetMesh(component["Mesh"].GetString());
    }

    if (component.HasMember("Material"))
    {
        m_pMaterial = pResources->GetMaterial(component["Material"].GetString());
    }
}

void MeshComponent::AddToInspector()
{
    ResourcesPanel::DropNodeMesh(&m_pMesh, m_pGameObject);
    ResourcesPanel::DropNodeMaterial(&m_pMaterial, m_pGameObject);
}

void MeshComponent::Init()
{
    ResourceManager* pResources = m_pGameObject->GetScene()->GetGameCore()->GetResourceManager();

    m_pMesh = pResources->GetMesh( "Cube" );
}

} // namespace fw
