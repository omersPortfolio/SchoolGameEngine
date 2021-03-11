#include "FrameworkPCH.h"
#include "TransformGizmo2D.h"

#include "Core/FWCore.h"
#include "Scene/Scene.h"
#include "Core/GameCore.h"
#include "Objects/Camera.h"
#include "Objects/GameObject.h"
#include "Objects/Mesh.h"
#include "Objects/ResourceManager.h"

namespace fw
{

void Gizmo2D::Draw(Camera* pCamera, vec3 pos, vec3 rot, vec3 scale, ShaderProgram* pShader, Texture* pTexture, vec2 UVScale, vec2 UVOffset)
{
    if (m_GizmoState == GizmoState::NONE)
    {
        m_pXAxisMesh->Draw(pCamera, pos, rot, scale, pShader, pTexture, Color::Red(), UVScale, UVOffset);
        m_pYAxisMesh->Draw(pCamera, pos, rot, scale, pShader, pTexture, Color::Blue(), UVScale, UVOffset);
        m_pOriginMesh->Draw(pCamera, pos, rot, scale, pShader, pTexture, Color::White(), UVScale, UVOffset);
    }

    if (m_GizmoState == GizmoState::BOTH)
    {
        m_pXAxisMesh->Draw(pCamera, pos, rot, scale, pShader, pTexture, Color::Yellow(), UVScale, UVOffset);
        m_pYAxisMesh->Draw(pCamera, pos, rot, scale, pShader, pTexture, Color::Yellow(), UVScale, UVOffset);
        m_pOriginMesh->Draw(pCamera, pos, rot, scale, pShader, pTexture, Color::Yellow(), UVScale, UVOffset);
    }

    if (m_GizmoState == GizmoState::X_AXIS)
    {
        m_pXAxisMesh->Draw(pCamera, pos, rot, scale, pShader, pTexture, Color::Yellow(), UVScale, UVOffset);
        m_pYAxisMesh->Draw(pCamera, pos, rot, scale, pShader, pTexture, Color::Blue(), UVScale, UVOffset);
        m_pOriginMesh->Draw(pCamera, pos, rot, scale, pShader, pTexture, Color::White(), UVScale, UVOffset);
    }

    if (m_GizmoState == GizmoState::Y_AXIS)
    {
        m_pXAxisMesh->Draw(pCamera, pos, rot, scale, pShader, pTexture, Color::Red(), UVScale, UVOffset);
        m_pYAxisMesh->Draw(pCamera, pos, rot, scale, pShader, pTexture, Color::Yellow(), UVScale, UVOffset);
        m_pOriginMesh->Draw(pCamera, pos, rot, scale, pShader, pTexture, Color::White(), UVScale, UVOffset);
    }
}

Gizmo2D::~Gizmo2D()
{
    delete m_pXAxisMesh;
    delete m_pYAxisMesh;
    delete m_pOriginMesh;
}

TransformGizmo2D::TransformGizmo2D(GameCore* pGameCore)
{
    m_pShader = pGameCore->GetResourceManager()->GetShader("Basic");
    m_pGameCore = pGameCore;

    Init(m_TranslationGizmo2D);
    Init(m_RotationGizmo2D);
    Init(m_ScaleGizmo2D);

    Construct(m_TranslationGizmo2D, m_RotationGizmo2D, m_ScaleGizmo2D);
}

TransformGizmo2D::~TransformGizmo2D()
{
}

void TransformGizmo2D::Draw()
{
    if (m_pSceneCamera)
    {
        if (m_pActiveGameObject)
        {
            m_ActiveGizmo->Draw(m_pSceneCamera, m_position, m_rotation, m_scale, m_pShader, nullptr, 1, 0);
        }
    }
}

void TransformGizmo2D::Update(Scene* pActiveScene)
{
    UpdateTransformGizmo(pActiveScene);
    //todo Improve code to draw Gizmo with constant size relative to screen
    //todo Add buttons for fine transformation

    static vec2 OldMousePosition = m_pSceneCamera->GetWorldSpaceMousePosition();
    vec2 MousePosition = m_pSceneCamera->GetWorldSpaceMousePosition();
    bool IsMouseDown = m_pGameCore->GetFramework()->IsMouseButtonDown(0);
    vec2 GizmoLoc = m_position.XY();
    vec2 DistanceFromGizmoCenter = vec2(MousePosition.x - GizmoLoc.x, MousePosition.y - GizmoLoc.y);

    if (!IsMouseDown)
    {
        if (DistanceFromGizmoCenter.x > m_scale * 0.05 && DistanceFromGizmoCenter.x < m_scale * 1.05)
        {
            if (abs(DistanceFromGizmoCenter.y) < m_scale * 0.05)
            {
                m_ActiveGizmo->m_GizmoState = GizmoState::X_AXIS;
            }

            else
            {
                m_ActiveGizmo->m_GizmoState = GizmoState::NONE;
            }
        }

        else if (DistanceFromGizmoCenter.y > m_scale * 0.05 && DistanceFromGizmoCenter.y < m_scale * 1.05)
        {
            if (abs(DistanceFromGizmoCenter.x) < m_scale * 0.05)
            {
                m_ActiveGizmo->m_GizmoState = GizmoState::Y_AXIS;
            }

            else
            {
                m_ActiveGizmo->m_GizmoState = GizmoState::NONE;
            }
        }

        else if (MousePosition.DistanceFrom(GizmoLoc) < m_scale * 0.05)
        {
            m_ActiveGizmo->m_GizmoState = GizmoState::BOTH;
        }

        else
        {
            m_ActiveGizmo->m_GizmoState = GizmoState::NONE;
        }
    }

    GameObject* pActiveGameObject = pActiveScene->GetSelectedGameObject();
    if (pActiveGameObject)
    {
        m_pActiveGameObject = pActiveGameObject;
        m_position = pActiveGameObject->GetPosition();

        if (IsMouseDown)
        {
            if (OldMousePosition != MousePosition)
            {
                vec2 DeltaPos = MousePosition - OldMousePosition;

                if (m_ActiveGizmo == &m_TranslationGizmo2D)
                {
                    vec3 oldpos = m_pActiveGameObject->GetPosition();

                    if (m_ActiveGizmo->m_GizmoState == GizmoState::BOTH)
                    {
                        vec3 newpos = oldpos + (MousePosition - OldMousePosition);
                        m_pActiveGameObject->SetPosition(newpos.XY());
                    }

                    if (m_ActiveGizmo->m_GizmoState == GizmoState::X_AXIS)
                    {
                        vec3 newpos = oldpos + DeltaPos.WithY(0.0f);
                        m_pActiveGameObject->SetPosition(newpos.XY());
                    }

                    if (m_ActiveGizmo->m_GizmoState == GizmoState::Y_AXIS)
                    {
                        vec3 newpos = oldpos + DeltaPos.WithX(0.0f);
                        m_pActiveGameObject->SetPosition(newpos.XY());
                    }
                }

                if (m_ActiveGizmo == &m_ScaleGizmo2D)
                {
                    vec3 oldscale = m_pActiveGameObject->GetScale();

                    if (m_ActiveGizmo->m_GizmoState == GizmoState::BOTH)
                    {
                        vec3 newscale = oldscale + (MousePosition - OldMousePosition);
                        m_pActiveGameObject->SetScale(newscale.XY());
                    }
                    if (m_ActiveGizmo->m_GizmoState == GizmoState::X_AXIS)
                    {
                        vec3 newscale = oldscale + DeltaPos.WithY(0.0f);
                        m_pActiveGameObject->SetScale(newscale.XY());
                    }
                    if (m_ActiveGizmo->m_GizmoState == GizmoState::Y_AXIS)
                    {
                        vec3 newscale = oldscale + DeltaPos.WithX(0.0f);
                        m_pActiveGameObject->SetScale(newscale.XY());
                    }
                }

                if (m_ActiveGizmo == &m_RotationGizmo2D)
                {
                    vec3 rot = m_pActiveGameObject->GetRotation();
                    float oldyaw = rot.z;
                    float amp = 40.0f;
                    float newyaw = rot.z + amp * (DeltaPos.x - DeltaPos.y);
                    rot.z = newyaw;
                    m_pActiveGameObject->SetRotation(rot);
                }
            }
        }
    }
    else
    {
        m_pActiveGameObject = nullptr;
    }

    bool MouseDown = m_pGameCore->GetFramework()->IsMouseButtonDown(0);
    OldMousePosition = m_pSceneCamera->GetWorldSpaceMousePosition();
}

void TransformGizmo2D::Init(Gizmo2D& gizmo_2d)
{
    gizmo_2d.m_pOriginMesh = new Mesh();
    gizmo_2d.m_pXAxisMesh = new Mesh();
    gizmo_2d.m_pYAxisMesh = new Mesh();
}

void TransformGizmo2D::Construct(Gizmo2D& Translation, Gizmo2D& Rotation, Gizmo2D& Scale)
{
    //todo Replace with better mesh
    {
        m_TranslationGizmo2D.m_pXAxisMesh->Start(GL_LINES);
        m_TranslationGizmo2D.m_pXAxisMesh->AddVertex(vec3(0.0f, 0.0f, .0f), vec2(0, 0));
        m_TranslationGizmo2D.m_pXAxisMesh->AddVertex(vec3(1.0f, 0.0f, .0f), vec2(0, 0));
        m_TranslationGizmo2D.m_pXAxisMesh->End();

        m_TranslationGizmo2D.m_pYAxisMesh->Start(GL_LINES);
        m_TranslationGizmo2D.m_pYAxisMesh->AddVertex(vec3(0.0f, 0.0f, .0f), vec2(0, 0));
        m_TranslationGizmo2D.m_pYAxisMesh->AddVertex(vec3(0.0f, 1.0f, .0f), vec2(0, 0));
        m_TranslationGizmo2D.m_pYAxisMesh->End();

        // TranslationGizmo2D.m_pOriginMesh->CreateCircle(0.5f);
    }

    {
        m_RotationGizmo2D.m_pXAxisMesh->Start(GL_LINES);
        m_RotationGizmo2D.m_pXAxisMesh->AddVertex(vec3(0.0f, 0.0f, .0f), vec2(0, 0));
        m_RotationGizmo2D.m_pXAxisMesh->AddVertex(vec3(1.0f, 0.0f, .0f), vec2(0, 0));
        m_RotationGizmo2D.m_pXAxisMesh->End();

        m_RotationGizmo2D.m_pYAxisMesh->Start(GL_LINES);
        m_RotationGizmo2D.m_pYAxisMesh->AddVertex(vec3(0.0f, 0.0f, .0f), vec2(0, 0));
        m_RotationGizmo2D.m_pYAxisMesh->AddVertex(vec3(0.0f, 1.0f, .0f), vec2(0, 0));
        m_RotationGizmo2D.m_pYAxisMesh->End();

        // RotationGizmo2D.m_pOriginMesh->CreateCircle(0.25f);
    }

    {
        m_ScaleGizmo2D.m_pXAxisMesh->Start(GL_LINES);
        m_ScaleGizmo2D.m_pXAxisMesh->AddVertex(vec3(0.0f, 0.0f, .0f), vec2(0, 0));
        m_ScaleGizmo2D.m_pXAxisMesh->AddVertex(vec3(1.0f, 0.0f, .0f), vec2(0, 0));
        m_ScaleGizmo2D.m_pXAxisMesh->End();

        m_ScaleGizmo2D.m_pYAxisMesh->Start(GL_LINES);
        m_ScaleGizmo2D.m_pYAxisMesh->AddVertex(vec3(0.0f, 0.0f, .0f), vec2(0, 0));
        m_ScaleGizmo2D.m_pYAxisMesh->AddVertex(vec3(0.0f, 1.0f, .0f), vec2(0, 0));
        m_ScaleGizmo2D.m_pYAxisMesh->End();

        m_ScaleGizmo2D.m_pOriginMesh->Start(GL_TRIANGLE_FAN);
        m_ScaleGizmo2D.m_pOriginMesh->AddVertex(vec3(0.05f, -0.05f, 0.0f), vec2(0, 0));
        m_ScaleGizmo2D.m_pOriginMesh->AddVertex(vec3(-0.05f, -0.05f, 0.0f), vec2(0, 0));
        m_ScaleGizmo2D.m_pOriginMesh->AddVertex(vec3(-0.05f, 0.05f, 0.0f), vec2(0, 0));
        m_ScaleGizmo2D.m_pOriginMesh->AddVertex(vec3(0.05f, 0.05f, 0.0f), vec2(0, 0));
        m_ScaleGizmo2D.m_pOriginMesh->End();
    }
}

void TransformGizmo2D::UpdateTransformGizmo(Scene* pActiveScene)
{
    m_pSceneCamera = pActiveScene->GetSceneCamera();
    m_pActiveGameObject = pActiveScene->GetSelectedGameObject();

    if (m_pGameCore->GetFramework()->IsKeyDown('W'))
    {
        m_ActiveGizmo = &m_TranslationGizmo2D;
        // m_rotation = 0;
    }

    if (m_pGameCore->GetFramework()->IsKeyDown('E'))
    {
        m_ActiveGizmo = &m_RotationGizmo2D;
        // m_rotation  = 0;
    }

    if (m_pGameCore->GetFramework()->IsKeyDown('R'))
    {
        m_ActiveGizmo = &m_ScaleGizmo2D;
        //todo Needs mouse to object collision detection to allow for rotating Gizmo
        // m_rotation = m_pActiveGameObject->GetRotation();
    }
}

} // namespace fw
