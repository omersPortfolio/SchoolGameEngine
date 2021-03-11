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
#include "Components/CollisionComponent.h"
#include "Scene/Scene.h"
#include "Components/ComponentManager.h"
#include "Material.h"
#include "Physics/PhysicsBody.h"
#include "ResourceManager.h"
#include "Utility/JSONHelpers.h"
#include "UI/ResourcesPanel.h"

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
    m_pScene->GetComponentManager()->Update(deltaTime);
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

    if (m_pMaterial != nullptr)
    {
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
    }

    writer.Key("Shader");
    writer.String("Basic");

    if (!m_Components.empty())
    {
        writer.Key("GameObjectComponents");
        writer.StartArray();

        for (Component* component : m_Components)
        {
            writer.StartObject();
            component->Save(writer);
            writer.EndObject();
        }

        writer.EndArray();
    }
}

void GameObject::AddComponent(Component* pComponent)
{
    pComponent->SetGameObject(this);
    pComponent->Init();
    m_pScene->GetComponentManager()->AddComponent(pComponent);
    m_Components.push_back(pComponent);
}

Component* GameObject::GetFirstComponentOfType(const char* type)
{
    for( Component* pComponent : m_Components )
    {
        if( pComponent->GetType() == type )
        {
            return pComponent;
        }
    }

    return nullptr;
}

void GameObject::ImGuiInspector()
{
    ImGui::PushID(m_Name.c_str());

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

        ImGui::Separator();
    }

    if (ImGui::DragFloat2("Position", &m_Position.x, 0.05f))
    {
        //for (Component* pComponent : m_Components)
        //{
        //    pComponent->SetPosition(m_Position);
        //}
    };
    ImGui::EndGroup();
    
    if (m_pMaterial)
    {
        Color tempColor = m_pMaterial->GetColor();
        if (ImGui::ColorEdit4("Color", &tempColor.r))
        {
            m_pMaterial->SetColor(tempColor);
        }

        if (ImGui::TreeNodeEx("Material Properties", ImGuiTreeNodeFlags_DefaultOpen))
        {
            std::string* assetName = ResourcesPanel::DropNodeImageM(m_pMaterial, "Material", "Drop Material", "Materials");
            if (assetName)
            {
                SetMaterial( m_pScene->GetGameCore()->GetResourceManager()->GetMaterial(*assetName) );
            }

            //if (ResourcesPanel::DropNodeImage(m_pMaterial->GetTexture(), "Texture", "Drop Texture", "Textures"))
            //    m_pMaterial->SetTexture(ResourcesPanel::SetTextureNode());

            //if (ResourcesPanel::DropNode("Shader", "Drop Shader", "Shaders"))
            //    m_pMaterial->SetShader(ResourcesPanel::SetShaderNode());

            ImGui::TreePop();
        }
    }

    for (Component* pComponent : m_Components)
    {
        pComponent->ImGuiInspector();
    }

    ImGui::PopID();
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
