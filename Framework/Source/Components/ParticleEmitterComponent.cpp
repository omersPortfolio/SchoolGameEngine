#include "FrameworkPCH.h"

#include "ParticleEmitterComponent.h"
#include "Particles/ParticleEmitter.h"
#include "Core/GameCore.h"
#include "Objects/GameObject.h"
#include "Objects/ResourceManager.h"
#include "Scene/Scene.h"

namespace fw
{

ParticleEmitterComponent::ParticleEmitterComponent()
{
    m_pEmitter = new fw::ParticleEmitter( nullptr );
}

ParticleEmitterComponent::ParticleEmitterComponent(Material* pMaterial)
{
    m_pEmitter = new fw::ParticleEmitter( pMaterial );
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
    delete m_pEmitter;
}

void ParticleEmitterComponent::Save(WriterType& writer)
{
    Component::Save(writer);

    ResourceManager* pResources = m_pGameObject->GetScene()->GetGameCore()->GetResourceManager();

    m_pEmitter->Save(writer, pResources);
}

void ParticleEmitterComponent::Load(rapidjson::Value& component)
{
    ResourceManager* pResources = m_pGameObject->GetScene()->GetGameCore()->GetResourceManager();

    m_pEmitter->Load(component, pResources);
}

void ParticleEmitterComponent::AddToInspector()
{
    m_pEmitter->AddToInspector(m_pGameObject->GetScene()->GetGameCore()->GetResourceManager());
}

void ParticleEmitterComponent::SetMaterial(fw::Material* pMaterial)
{
    m_pEmitter->SetMaterial( pMaterial );
}

} // namespace fw
