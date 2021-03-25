#include "FrameworkPCH.h"

#include "GameObject.h"
#include "Core/GameCore.h"
#include "Components/AABBComponent.h"
#include "Components/CameraComponent.h"
#include "Components/CollisionComponent.h"
#include "Components/ComponentManager.h"
#include "Components/MeshComponent.h"
#include "Components/TransformComponent.h"
#include "EventSystem/EventManager.h"
#include "Objects/AABB.h"
#include "Objects/Material.h"
#include "Objects/Mesh.h"
#include "Objects/ResourceManager.h"
#include "Physics/PhysicsBody.h"
#include "Scene/Scene.h"
#include "UI/ResourcesPanel.h"
#include "Utility/JSONHelpers.h"

namespace fw {

GameObject::GameObject(Scene* pScene, std::string name)
{
    m_pScene = pScene;
    m_Name = name;
}

GameObject::GameObject(GameObject& orgnlGO)
{
    *this = orgnlGO;
    SetName(std::string("New ") + GetName().c_str());
    m_pTransform = nullptr;

    m_Components.clear();

    for (int i = 0; i < orgnlGO.m_Components.size(); i++)
    {
        if (orgnlGO.m_Components[i]->GetType() == TransformComponent::GetStaticType())
        {
            TransformComponent* cComponent = static_cast<TransformComponent*>(orgnlGO.m_Components[i]);

            TransformComponent* nComponent = new TransformComponent();
            nComponent->SetPosition(cComponent->GetPosition());
            nComponent->SetRotation(cComponent->GetRotation());
            nComponent->SetScale(cComponent->GetScale());

            this->AddComponent(nComponent);
            m_pTransform = nComponent;
        }
        if (orgnlGO.m_Components[i]->GetType() == MeshComponent::GetStaticType())
        {
            MeshComponent* cComponent = static_cast<MeshComponent*>(orgnlGO.m_Components[i]);
            this->AddComponent(new MeshComponent(cComponent->GetMesh(), cComponent->GetMaterial()));
        }
        if (orgnlGO.m_Components[i]->GetType() == AABBComponent::GetStaticType())
        {
            AABBComponent* cComponent = static_cast<AABBComponent*>(orgnlGO.m_Components[i]);
            //this->AddComponent(new AABBComponent(cComponent->GetAABB()));
        }
        if (orgnlGO.m_Components[i]->GetType() == CollisionComponent::GetStaticType())
        {
            CollisionComponent* cComponent = static_cast<CollisionComponent*>(orgnlGO.m_Components[i]);
            this->AddComponent(new CollisionComponent(cComponent));
        }
    }
}

GameObject::~GameObject()
{
    if (m_pScene != nullptr)
    {
        m_pScene->GetGameCore()->GetEventManager()->UnregisterEventListenerCompletely(this);

        for (Component* pComp : m_Components)
        {
            m_pScene->GetComponentManager()->RemoveComponent(pComp);
            delete pComp;
        }
    }
}

void GameObject::Update(float deltaTime)
{
}

void GameObject::Draw(CameraComponent* pCamera)
{
    //m_pMesh->Draw( pCamera, m_Position, m_pShader, m_pTexture, m_Color, m_UVScale, m_UVOffset );
}

void GameObject::Save(WriterType& writer)
{
    ResourceManager* pResources = m_pScene->GetGameCore()->GetResourceManager();

    writer.Key("Name");
    writer.String(m_Name.c_str());

    if (!m_Components.empty())
    {
        writer.Key("GameObjectComponents");
        writer.StartArray();

        for (Component* pComponent : m_Components)
        {
            writer.StartObject();
            pComponent->Save(writer);
            writer.EndObject();
        }

        writer.EndArray();
    }
}

void GameObject::AddComponent(Component* pComponent)
{
    // Make sure there is only 1 Transform component.
    if (pComponent->GetType() == TransformComponent::GetStaticType() && m_pTransform)
    {
        return;
    }

    pComponent->SetGameObject(this);
    pComponent->Init();
    m_pScene->GetComponentManager()->AddComponent(pComponent);
    m_Components.push_back(pComponent);
}

Component* GameObject::GetFirstComponentOfType(const char* type)
{
    for (Component* pComponent : m_Components)
    {
        if (pComponent->GetType() == type)
        {
            return pComponent;
        }
    }

    return nullptr;
}

TransformComponent* GameObject::GetTransform()
{
    Component* pComponent = GetFirstComponentOfType(TransformComponent::GetStaticType());
    return static_cast<TransformComponent*>(pComponent);
}

void GameObject::AddToInspector()
{
    // Display name and allow for renaming.
    ImGui::BeginGroup();
    {
        const int maxNameLen = 32;
        char text[maxNameLen];
        strncpy_s(text, maxNameLen, m_Name.c_str(), maxNameLen);
        if (ImGui::InputText("Name", text, maxNameLen))
        {
            m_Name = text;
        }
    }

    for (Component* pComponent : m_Components)
    {
        if (ImGui::CollapsingHeader(pComponent->GetType(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::PushID(pComponent);
            pComponent->AddToInspector();
            ImGui::PopID();
        }
    }
    if (m_Components.empty())
    {
        ImGui::Text("Object has no Components.");
    }
    ImGui::EndGroup();
}

#if FW_USING_LUA
void GameObject::LuaRegister(lua_State* luastate)
{
    luabridge::getGlobalNamespace(luastate)
        .beginClass<GameObject>("GameObject")
        .addProperty("position", &GameObject::m_Position)
        .endClass();
}
#endif

} // namespace fw
