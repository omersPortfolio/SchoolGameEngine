#pragma once
#include "Math/Vector.h"

namespace fw
{

class Mesh;
class Camera;
class vec3;
class ShaderProgram;
class Texture;
class GameCore;
class Scene;
class GameObject;

enum class GizmoState
{
    NONE,
    X_AXIS,
    Y_AXIS,
    BOTH
};

enum class TransformMode
{
    TRANSLATION,
    ROTATION,
    SCALE
};

struct Gizmo2D
{
    Mesh* m_pXAxisMesh;
    Mesh* m_pYAxisMesh;
    Mesh* m_pOriginMesh;

    GizmoState m_GizmoState = GizmoState::NONE;
    void Draw(Camera* pCamera, vec3 pos, vec3 rot, vec3 scale, ShaderProgram* pShader, class Texture* pTexture, vec2 UVScale, vec2 UVOffset);

    ~Gizmo2D();
};

class TransformGizmo2D
{
public:
    TransformGizmo2D(GameCore* pGameCore);
    ~TransformGizmo2D();

    void Draw();
    void Update(Scene* pActiveScene);

private:
    void Init(Gizmo2D& gizmo_2d);
    void Construct(Gizmo2D& Translation, Gizmo2D& Rotation, Gizmo2D& Scale);
    void UpdateTransformGizmo(Scene* pActiveScene);

    Gizmo2D m_TranslationGizmo2D;
    Gizmo2D m_RotationGizmo2D;
    Gizmo2D m_ScaleGizmo2D;

    Gizmo2D* m_ActiveGizmo = &m_TranslationGizmo2D;

    vec3 m_position = 0;
    vec3 m_rotation = 0;
    float m_scale = 2;

    TransformMode m_TransformMode = TransformMode::TRANSLATION;

    GameCore* m_pGameCore = nullptr;
    ShaderProgram* m_pShader = nullptr;
    Camera* m_pSceneCamera = nullptr;
    GameObject* m_pActiveGameObject = nullptr;
};

}


