#include "FrameworkPCH.h"
#include "ComponentManager.h"
#include "MeshComponent.h"
#include "AABBComponent.h"
#include "Objects/GameObject.h"
#include "Objects/Mesh.h"
#include "Objects/AABB.h"

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

        //GameObject* pGO = pAABBComponent->GetGameObject();
        //pAABBComponent->GetAABB().Set(pGO->GetPosition(), vec3(1, 1, 1));
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
        pMeshComponent->GetMesh()->Draw(pCamera,
                                        pGO->GetPosition(), pGO->GetRotation(), pGO->GetScale(),
                                        pGO->GetMaterial());
    }
}

void ComponentManager::DrawDebugAABBComponents(Camera* pCamera)
{
    std::vector<Component*>& list = m_Components[AABBComponent::GetStaticType()];
    for (int i = 0; i < list.size(); i++)
    {
        AABBComponent* pAABBComponent = static_cast<AABBComponent*>(list[i]);

        GameObject* pGO = pAABBComponent->GetGameObject();
        pAABBComponent->GetMesh()->Draw(pCamera,
                                        pGO->GetPosition(), pGO->GetRotation(), pGO->GetScale(),
                                        pGO->GetMaterial());
    }
}

} // namespace fw
