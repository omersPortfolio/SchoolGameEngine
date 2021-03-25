#include "FrameworkPCH.h"

#include "ComponentManager.h"
#include "Components/AABBComponent.h"
#include "Components/CameraComponent.h"
#include "Components/CollisionComponent.h"
#include "Components/LuaScriptComponent.h"
#include "Components/MeshComponent.h"
#include "Components/ParticleEmitterComponent.h"
#include "Components/TransformComponent.h"
#include "Lua/LuaGameState.h"
#include "Objects/GameObject.h"
#include "Objects/Mesh.h"
#include "Objects/AABB.h"
#include "Particles/ParticleEmitter.h"
#include "Scene/Scene.h"

namespace fw {

ComponentManager::ComponentManager()
{
}

ComponentManager::~ComponentManager()
{
}

void ComponentManager::Draw(CameraComponent* pCamera)
{
    DrawMeshComponents(pCamera);
    DrawParticleEmitterComponents(pCamera);
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

    UpdateParticleEmitterComponents(deltaTime);
}

void ComponentManager::AddToInspector()
{
    std::vector<Component*>& collisionComponentsList = m_Components[CollisionComponent::GetStaticType()];
    for (int i = 0; i < collisionComponentsList.size(); i++)
    {
        CollisionComponent* pCollisionComponent = static_cast<CollisionComponent*>(collisionComponentsList[i]);

        pCollisionComponent->AddToInspector();
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

void ComponentManager::UpdateParticleEmitterComponents(float deltaTime)
{
    std::vector<Component*>& list = m_Components[ParticleEmitterComponent::GetStaticType()];
    for (int i = 0; i < list.size(); i++)
    {
        ParticleEmitterComponent* pEmitterComponent = static_cast<ParticleEmitterComponent*>(list[i]);

        ParticleEmitter* pEmitter = pEmitterComponent->GetEmitter();
        pEmitter->Update(deltaTime);
    }
}

void ComponentManager::DrawMeshComponents(CameraComponent* pCamera)
{
    std::vector<Component*>& list = m_Components[MeshComponent::GetStaticType()];
    for (int i = 0; i < list.size(); i++)
    {
        MeshComponent* pMeshComponent = static_cast<MeshComponent*>(list[i]);

        GameObject* pGO = pMeshComponent->GetGameObject();

        if( pGO != nullptr && pGO->GetTransform() &&
           pMeshComponent->GetMesh() && pMeshComponent->GetMaterial() )
        {
            pMeshComponent->GetMesh()->Draw(pCamera,
                                            pGO->GetTransform(),
                                            pMeshComponent->GetMaterial());
        }
    }
}

void ComponentManager::DrawParticleEmitterComponents(CameraComponent* pCamera)
{
    std::vector<Component*>& list = m_Components[ParticleEmitterComponent::GetStaticType()];
    for (int i = 0; i < list.size(); i++)
    {
        ParticleEmitterComponent* pEmitterComponent = static_cast<ParticleEmitterComponent*>(list[i]);

        GameObject* pGO = pEmitterComponent->GetGameObject();
        ParticleEmitter* pEmitter = pEmitterComponent->GetEmitter();
        pEmitter->Draw(pCamera, pGO->GetTransform()->GetPosition());
    }
}

void ComponentManager::DrawDebugAABBComponents(CameraComponent* pCamera)
{
    std::vector<Component*>& list = m_Components[AABBComponent::GetStaticType()];
    for (int i = 0; i < list.size(); i++)
    {
        AABBComponent* pAABBComponent = static_cast<AABBComponent*>(list[i]);

        GameObject* pGO = pAABBComponent->GetGameObject();
        if( pAABBComponent->GetMesh() )
        {
            //pAABBComponent->GetMesh()->Draw(pCamera,
            //                                pGO->GetTransform(),
            //                                pGO->GetMaterial());
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
