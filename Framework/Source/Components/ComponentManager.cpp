#include "FrameworkPCH.h"
#include "ComponentManager.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "AABBComponent.h"
#include "Objects/GameObject.h"
#include "Objects/Mesh.h"
#include "Objects/AABB.h"
#include "LuaScriptComponent.h"
#include "Scene/Scene.h"
#include "Lua/LuaGameState.h"

namespace fw {

ComponentManager::ComponentManager()
{
}

ComponentManager::~ComponentManager()
{
}

void ComponentManager::Draw(Camera* pCamera)
{
    DrawMeshComponents(pCamera);
    DrawDebugAABBComponents(pCamera);
}

void ComponentManager::Update(float deltaTime)
{
    std::vector<Component*>& list = m_Components[AABBComponent::GetStaticType()];
    for (int i = 0; i < list.size(); i++)
    {
        AABBComponent* pAABBComponent = static_cast<AABBComponent*>(list[i]);
 
    }

#if FW_USING_LUA
    UpdateLuaScriptComponents(deltaTime);
#endif

    std::vector<Component*>& collisionComponentsList = m_Components[CollisionComponent::GetStaticType()];
    for (int i = 0; i < collisionComponentsList.size(); i++)
    {
        CollisionComponent* pCollisionComponent = static_cast<CollisionComponent*>(collisionComponentsList[i]);

        pCollisionComponent->Update(deltaTime);
    }
}

void ComponentManager::ImGuiInspector()
{
    std::vector<Component*>& collisionComponentsList = m_Components[CollisionComponent::GetStaticType()];
    for (int i = 0; i < collisionComponentsList.size(); i++)
    {
        CollisionComponent* pCollisionComponent = static_cast<CollisionComponent*>(collisionComponentsList[i]);

        pCollisionComponent->ImGuiInspector();
    }
}

void ComponentManager::AddComponent(Component* pComponent)
{
    assert(pComponent != nullptr);

    const char* type = pComponent->GetType();
    m_Components[type].push_back(pComponent);
}

void ComponentManager::RemoveComponent(Component* pComponent)
{
    const char* type = pComponent->GetType();

    std::vector<Component*>& list = m_Components[type];
    list.erase(std::remove(list.begin(), list.end(), pComponent), list.end());
}

std::vector<Component*>& ComponentManager::GetComponentList(const char* type)
{
    return m_Components[type];
}

void ComponentManager::DrawMeshComponents(Camera* pCamera)
{
    std::vector<Component*>& list = m_Components[MeshComponent::GetStaticType()];
    for (int i = 0; i < list.size(); i++)
    {
        MeshComponent* pMeshComponent = static_cast<MeshComponent*>(list[i]);

        GameObject* pGO = pMeshComponent->GetGameObject();
        if( pMeshComponent->GetMesh() )
        {
            pMeshComponent->GetMesh()->Draw(pCamera,
                                            pGO->GetPosition(), pGO->GetRotation(), pGO->GetScale(),
                                            pGO->GetMaterial());
        }
    }
}

void ComponentManager::DrawDebugAABBComponents(Camera* pCamera)
{
    std::vector<Component*>& list = m_Components[AABBComponent::GetStaticType()];
    for (int i = 0; i < list.size(); i++)
    {
        AABBComponent* pAABBComponent = static_cast<AABBComponent*>(list[i]);

        GameObject* pGO = pAABBComponent->GetGameObject();
        if( pAABBComponent->GetMesh() )
        {
            pAABBComponent->GetMesh()->Draw(pCamera,
                                            pGO->GetPosition(), pGO->GetRotation(), pGO->GetScale(),
                                            pGO->GetMaterial());
        }
    }
}

#if FW_USING_LUA
void ComponentManager::UpdateLuaScriptComponents(float deltaTime)
{
    std::vector<Component*>& list = m_Components[LuaScriptComponent::GetStaticType()];
    for (int i = 0; i < list.size(); i++)
    {
        LuaScriptComponent* pLuaComponent = static_cast<LuaScriptComponent*>(list[i]);

        GameObject* pGO = pLuaComponent->GetGameObject();
        pGO->GetScene()->GetLuaState()->UpdateGameObjectFromLua(pGO, deltaTime, "Tick");
    }
}
#endif

} // namespace fw
