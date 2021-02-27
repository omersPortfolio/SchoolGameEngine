#include "FrameworkPCH.h"

#include "ParticleEmitter.h"
#include "Objects/Mesh.h"
#include "Objects/Material.h"

namespace fw {

ParticleEmitter::ParticleEmitter(Material* pMaterial)
{
    m_pMesh = new Mesh();
    m_pMaterial = pMaterial;

    m_NewParticleTimer = 0.5f;
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
        m_Particles[m_Particles.size() - 1].m_TimeToLive = 3.0f;
        float angle = (rand() % RAND_MAX / (float)RAND_MAX) * (float)PI * 2;
        vec2 dir(cos(angle), sin(angle));
        float speed = (rand() % RAND_MAX / (float)RAND_MAX) * 2 + 1;
        m_Particles[m_Particles.size() - 1].m_Velocity = dir * speed;
        m_NewParticleTimer = 0.5f;
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

void ParticleEmitter::Draw(Camera* pCamera)
{
    m_pMesh->Draw(pCamera, 0, 0, 1, m_pMaterial);
}

} // namespace fw
