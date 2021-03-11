#include "FrameworkPCH.h"
#include "ComponentRegistry.h"

#include <array>

#include "ComponentManager.h"
#include "MeshComponent.h"
#include "AABBComponent.h"
#include "Objects/GameObject.h"
#include "Objects/Mesh.h"
#include "Objects/AABB.h"

namespace fw {

ComponentRegistry::ComponentRegistry()
{
    Register(MeshComponent::GetStaticType(), &MeshComponent::Create);
    Register(AABBComponent::GetStaticType(), &AABBComponent::Create);
}

void ComponentRegistry::Register(const std::string& Name, CreateComponentFn pfnCreate)
{
    m_ComponentTypes[Name] = pfnCreate;
}

Component* ComponentRegistry::GetComponent(const std::string& componentName)
{
    std::map<const std::string, CreateComponentFn>::iterator it = m_ComponentTypes.find(componentName);
    if (it != m_ComponentTypes.end())
    {
        return it->second();
    }
    return NULL;
}

void ComponentRegistry::AddInterfaceToCreateComponents(GameObject* pObject)
{
    for (auto& entry : m_ComponentTypes)
    {
        if (ImGui::Button(entry.first.c_str()))
        {
            pObject->AddComponent(entry.second());

            LOG(INFO, "%s added to Object %s ", entry.first.c_str(), pObject->GetName().c_str());
        }
    }
}

} // namespace fw
