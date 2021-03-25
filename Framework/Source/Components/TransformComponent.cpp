#include "FrameworkPCH.h"

#include "TransformComponent.h"
#include "Objects/GameObject.h"
#include "Component.h"

namespace fw {

TransformComponent::TransformComponent()
{
    m_Position.Set(0, 0, 0);
    m_Rotation.Set(0, 0, 0);
    m_Scale.Set(1, 1, 1);
}

TransformComponent::TransformComponent(vec3 pos, vec3 rot, vec3 scale)
{
    m_Position = pos;
    m_Rotation = rot;
    m_Scale = scale;
}

void TransformComponent::Save(WriterType& writer)
{
    Component::Save(writer);

    JSONSaveVec3(writer, "Pos", m_Position);
    JSONSaveVec3(writer, "Rot", m_Rotation);
    JSONSaveVec3(writer, "Scale", m_Scale);

    if (m_pParent)
    {
        writer.Key("Parent");
        writer.String(m_pParent->GetName().c_str());
        //writer.int(m_pParent->GetUniqueID()); Incase gameobject names are allowed to be similar(like in unity)  
    }
}

void TransformComponent::Load(rapidjson::Value& component)
{
    rapidjson::Document loadedScene;

    JSONLoadVec3(component, "Pos", &m_Position);
    JSONLoadVec3(component, "Rot", &m_Rotation);
    JSONLoadVec3(component, "Scale", &m_Scale);

    /*
    *Issue:at this point not all objects have been loaded in memory so assigning childs are impossible
    *PtnlSolution:An event that calls a functions to all object once scene has been loaded(like Start() in unity)

    int childCount = (int)gameObject["Color"].Size();
     std::string parentName;

     for (int i = 0; i < m_Owner->GetScene().m_Objects.Size(); i++)
     {
         if (m_Owner->GetScene().m_Objects[i]->GetName() == parentName)
         {
             m_Owner->GetScene().m_Objects[i]->GetTransform()->SetChild(m_Owner);
         }
     }

     */
}

MyMatrix* TransformComponent::GetWorldTransform()
{
    m_WorldMatrix.CreateSRT(m_Scale, m_Rotation, m_Position);
    if (m_pParent)
    {
        MyMatrix* pParentMat = m_pParent->GetTransform()->GetWorldTransform();
        m_WorldMatrix = *pParentMat * m_WorldMatrix;
    }

    return &m_WorldMatrix;
}

void TransformComponent::SetParent(GameObject* aParent)
{
    if (m_pParent != nullptr)
    {
        m_pParent->GetTransform()->RemoveChild(m_pGameObject);
        m_pParent = nullptr;
    }
    m_pParent = aParent;
}

void TransformComponent::SetChild(GameObject* aChild)
{
    m_Children.push_back(aChild); aChild->GetTransform()->SetParent(m_pGameObject);
}

void TransformComponent::RemoveChild(GameObject* aChild)
{
    for (int i = 0; i < m_Children.size(); i++)
    {
        if (m_Children[i] == aChild)
        {
            m_Children.erase(m_Children.begin() + i);
        }
    }
}

void TransformComponent::AddToInspector()
{
    if (ImGui::DragFloat3("Position", &m_Position.x, 0.05f))
    {
        //m_pGameObject->SetPositionPhysicsBody();
    };

    ImGui::DragFloat3("Rotation", &m_Rotation.x, 0.5f);
    ImGui::DragFloat3("Scale", &m_Scale.x, 0.05f);
}

} // namespace fw
