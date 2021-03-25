#include "FrameworkPCH.h"

#include "ParticleEmitter.h"
#include "Objects/Mesh.h"
#include "Objects/Material.h"
#include "Objects/ResourceManager.h"
#include "UI/ResourcesPanel.h"
#include "Math/MyMatrix.h"

namespace fw {

ParticleEmitter::ParticleEmitter(Material* pMaterial)
{
    m_pMesh = new Mesh();
    m_pMaterial = pMaterial;

    m_NewParticleTimerDuration = 2.0f;
    m_NewParticleTimer = m_NewParticleTimerDuration;
    m_ParticleSpeed = 1.0f;
    m_ParticleLifespan = 3.0f;
    m_ParticleColor = Color::White();
    m_Origin = vec3(0);
}

ParticleEmitter::~ParticleEmitter()
{
    delete m_pMesh;
}

void ParticleEmitter::Update(float deltaTime)
{
    // Move existing particles.
    for (int i = 0; i < m_Particles.size(); i++)
    {
        m_Particles[i].m_Position += m_Particles[i].m_Velocity * deltaTime;

        m_Particles[i].m_TimeToLive -= deltaTime;
        if (m_Particles[i].m_TimeToLive <= 0)
        {
            m_Particles[i] = m_Particles[m_Particles.size() - 1];
            m_Particles.pop_back();
            i--;
        }
    }

    // Create new particles.
    m_NewParticleTimer -= deltaTime;
    if (m_NewParticleTimer <= 0)
    {
        m_Particles.push_back(Particle());
        m_Particles[m_Particles.size() - 1].m_Position = 0;
        m_Particles[m_Particles.size() - 1].m_TimeToLive = m_ParticleLifespan;
        float angle = (rand() % RAND_MAX / (float)RAND_MAX) * (float)PI * 2;
        vec2 dir(cos(angle), sin(angle));
        m_Particles[m_Particles.size() - 1].m_Velocity = dir * m_ParticleSpeed;
        m_NewParticleTimer = m_NewParticleTimerDuration;
    }

    // Create the mesh.
    if (m_Particles.size() > 0)
    {
        m_pMesh->Start(GL_TRIANGLES);
        for (Particle& particle : m_Particles)
        {
            m_pMesh->AddSprite(particle.m_Position);
        }
        m_pMesh->End();
    }
}

void ParticleEmitter::Draw(CameraComponent* pCamera, vec3 position)
{
    if( m_pMaterial == nullptr )
        return;

    MyMatrix identity;
    identity.CreateTranslation(position + m_Origin);
    m_pMesh->Draw(pCamera, &identity, m_pMaterial);
}

void ParticleEmitter::Save(WriterType& writer, ResourceManager* pResources)
{
    JSONSaveVec3(writer, "Origin", m_Origin);

    writer.Key("TimerDuration");
    writer.Double(m_NewParticleTimerDuration);

    writer.Key("Speed");
    writer.Double(m_ParticleSpeed);

    writer.Key("Lifespan");
    writer.Double(m_ParticleLifespan);

    JSONSaveCharArray(writer, "Material", pResources->FindMaterialName(m_pMaterial).c_str());
}

void ParticleEmitter::Load(rapidjson::Value& component, ResourceManager* pResources)
{
    if (component.HasMember("Origin"))
    {
        m_Origin.x = (float)component["Origin"].GetArray()[0].GetDouble();
        m_Origin.y = (float)component["Origin"].GetArray()[1].GetDouble();
        m_Origin.z = (float)component["Origin"].GetArray()[2].GetDouble();
    }

    if (component.HasMember("TimerDuration"))
    {
        m_NewParticleTimerDuration = (float)component["TimerDuration"].GetDouble();
    }

    if (component.HasMember("Speed"))
    {
        m_ParticleSpeed = (float)component["Speed"].GetDouble();
    }

    if (component.HasMember("Lifespan"))
    {
        m_ParticleLifespan = (float)component["Lifespan"].GetDouble();
    }

    if (component.HasMember("Color"))
    {
        m_ParticleColor.r = (float)component["Color"].GetArray()[0].GetDouble();
        m_ParticleColor.g = (float)component["Color"].GetArray()[1].GetDouble();
        m_ParticleColor.b = (float)component["Color"].GetArray()[2].GetDouble();
        m_ParticleColor.a = (float)component["Color"].GetArray()[3].GetDouble();
    }

    if (component.HasMember("Material"))
    {
        m_pMaterial = pResources->GetMaterial(component["Material"].GetString());
    }
}

void ParticleEmitter::AddToInspector(ResourceManager* pResourceManager)
{
    ImGui::DragFloat3("Origin", &m_Origin.x, 0.05f);
    ImGui::DragFloat("Spawn Interval", &m_NewParticleTimerDuration, 0.05f, 0, 10);
    ImGui::DragFloat("Speed", &m_ParticleSpeed, 0.05f, 0, 50);
    ImGui::DragFloat("Lifespan", &m_ParticleLifespan, 0.1f, 0, 100);

    if (m_pMaterial)
    {
        std::string* matNameDropped = ResourcesPanel::DropNode("Material", "", "Materials");
        if (matNameDropped)
        {
            m_pMaterial = pResourceManager->GetMaterial(*matNameDropped);
        }
        //ImGui::DragFloat4("Color", &m_ParticleColor.r, 0, 0, 1);
        //m_pMaterial->SetColor(m_ParticleColor);
    }
}

} // namespace fw
