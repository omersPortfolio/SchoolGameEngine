#pragma once

#include "Math/Vector.h"
#include "Utility/Color.h"
#include "Utility/JSONHelpers.h"

namespace fw {

class CameraComponent;
class Material;
class Mesh;
class ResourceManager;

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
    void Draw(CameraComponent* pCamera, vec3 position);

    void Save(WriterType& writer, ResourceManager* pResources);
    virtual void Load(rapidjson::Value& component, ResourceManager* pResources);

    void AddToInspector(ResourceManager* pResourceManager);

    void SetMaterial(Material* pMaterial) { m_pMaterial = pMaterial; }

protected:
    std::vector<Particle> m_Particles;

    float m_NewParticleTimer;
    float m_NewParticleTimerDuration;
    float m_ParticleSpeed;
    float m_ParticleLifespan;
    //float m_EmissionAngleMin;
    //float m_EmissionAngleMax;
    vec3 m_Origin;
    bool m_IsLooping; //not sure how to implement
    Color m_ParticleColor; //crashes

    Mesh* m_pMesh;
    Material* m_pMaterial;
};
} // namespace fw
