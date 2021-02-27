#pragma once

#include "Math/Vector.h"

namespace fw {

class Camera;
class Material;
class Mesh;

class ParticleEmitter
{
    struct Particle
    {
        vec3 m_Position;
        vec3 m_Velocity;
        float m_TimeToLive;
    };

public:
    ParticleEmitter(Material* pMaterial);
    virtual ~ParticleEmitter();

    void Update(float deltaTime);
    void Draw(Camera* pCamera);

protected:
    std::vector<Particle> m_Particles;

    float m_NewParticleTimer;

    Mesh* m_pMesh;
    Material* m_pMaterial;
};

} // namespace fw
