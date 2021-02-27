#include "FrameworkPCH.h"

#include "EventSystem/EventManager.h"
#include "Scene/Scene.h"
#include "Core/GameCore.h"
#include "Objects/AABB.h"

#include "GameObject.h"
#include "Mesh.h"
#include "Scene/Scene.h"
#include "Components/MeshComponent.h"
#include "Components/AABBComponent.h"
#include "Scene/Scene.h"
#include "Components/ComponentManager.h"
#include "Material.h"
#include "Physics/PhysicsBody.h"
#include "ResourceManager.h"
#include "Utility/JSONHelpers.h"

namespace fw {

GameObject::GameObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, Material* pMaterial)
{
    m_pScene = pScene;
    m_Name = name;

    m_Position = pos;
    m_Rotation = rot;
    m_Scale = scale;

    m_pMaterial = pMaterial;
}

GameObject::GameObject(GameObject& orgnlGO)
{
    *this = orgnlGO;
    SetName(std::string("New ") + GetName().c_str());

    m_Components.clear();
    for (int i = 0; i < orgnlGO.m_Components.size(); i++)
    {
        if (orgnlGO.m_Components[i]->GetType() == MeshComponent::GetStaticType())
        {
            MeshComponent* cComponent = static_cast<MeshComponent*>(orgnlGO.m_Components[i]);
            this->AddComponent(new MeshComponent(cComponent->GetMesh()));
        }
        if (orgnlGO.m_Components[i]->GetType() == AABBComponent::GetStaticType())
        {
            AABBComponent* cComponent = static_cast<AABBComponent*>(orgnlGO.m_Components[i]);
            //this->AddComponent(new AABBComponent(cComponent->GetAABB()));
        }
    }

    // Not proper duplication,can be easily implemented once Physicsbody is made a component.
    CreatePhysicsBodyBox(fw::BodyType::StaticBody, 1, 1);
}

GameObject::~GameObject()
{
    m_pScene->GetGameCore()->GetEventManager()->UnregisterEventListenerCompletely(this);

    for (Component* pComp : m_Components)
    {
        m_pScene->GetComponentManager()->RemoveComponent(pComp);
        delete pComp;
    }

    delete m_pPhysicsBody;
}

void GameObject::Update(float deltaTime)
{
    if (m_pPhysicsBody)
    {
        m_Position = m_pPhysicsBody->GetPosition().XY();
        //m_Rotation = m_pPhysicsBody->GetRotation().z;
    }
}

void GameObject::Draw(Camera* pCamera)
{
    //m_pMesh->Draw( pCamera, m_Position, m_pShader, m_pTexture, m_Color, m_UVScale, m_UVOffset );
}

void GameObject::Save(WriterType& writer)
{
    ResourceManager* pResources = m_pScene->GetGameCore()->GetResourceManager();

    writer.Key("Name");
    writer.String(m_Name.c_str());

    JSONSaveVec3(writer, "Pos", m_Position);
    JSONSaveVec3(writer, "Rot", m_Rotation);
    JSONSaveVec3(writer, "Scale", m_Scale);

    writer.Key("Textures");
    writer.String(pResources->FindTextureName(m_pMaterial->GetTexture()).c_str());

    writer.Key("Color");
    writer.StartArray();
    writer.Double(m_pMaterial->GetColor().r);
    writer.Double(m_pMaterial->GetColor().g);
    writer.Double(m_pMaterial->GetColor().b);
    writer.Double(m_pMaterial->GetColor().a);
    writer.EndArray();

    JSONSaveVec2(writer, "uvScale", m_pMaterial->GetUVScale());
    JSONSaveVec2(writer, "uvOffset", m_pMaterial->GetUVOffset());

    writer.Key("Shader");
    writer.String("Basic");
}

void GameObject::AddComponent(Component* pComponent)
{
    pComponent->SetGameObject(this);
    m_pScene->GetComponentManager()->AddComponent(pComponent);
    m_Components.push_back(pComponent);
}

void GameObject::CreatePhysicsBodyCircle(BodyType bodyType, float radius, float density)
{
    m_pPhysicsBody = m_pScene->GetWorld()->CreateBody(m_Position, bodyType, true);
    m_pPhysicsBody->CreateCircle(radius);
    m_pPhysicsBody->SetDensity(density);
}

void GameObject::CreatePhysicsBodyBox(BodyType bodyType, float boxWidth, float boxHeight, float density)
{
    m_pPhysicsBody = m_pScene->GetWorld()->CreateBody(m_Position, bodyType, true);
    m_pPhysicsBody->CreateBox(boxWidth, boxHeight);
    m_pPhysicsBody->SetDensity(density);
}

void GameObject::SetPositionPhysicsBody()
{
    if (m_pPhysicsBody)
    {
        m_pPhysicsBody->SetPosition(m_Position);
        //m_pPhysicsBody->SetRotation(m_Rotation);
    }
}

void GameObject::ImGuiInspector()
{
    // Display name and allow for renaming.
    {
        const int maxNameLen = 32;
        char text[maxNameLen];
        strncpy_s(text, maxNameLen, m_Name.c_str(), maxNameLen);
        if (ImGui::InputText("Name", text, maxNameLen))
        {
            m_Name = text;
        }

        ImGui::Separator();
    }

    if (ImGui::DragFloat2("Position", &m_Position.x, 0.05f))
    {
        if (m_pPhysicsBody)
        {
            SetPositionPhysicsBody();
        }
    };

    if (m_pMaterial)
    {
        Color tempColor = m_pMaterial->GetColor();
        if (ImGui::ColorEdit4("Color", &tempColor.r))
        {
            m_pMaterial->SetColor(tempColor);
        }
    }
}

} // namespace fw
