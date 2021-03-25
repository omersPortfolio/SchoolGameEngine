#include "FrameworkPCH.h"

#include "CameraComponent.h"
#include "Components/TransformComponent.h"
#include "Core/GameCore.h"
#include "Core/FWCore.h"
#include "Math/MyMatrix.h"
#include "Objects/GameObject.h"
#include "Scene/Scene.h"
#include "Utility/JSONHelpers.h"
#include "EventSystem/EventManager.h"
#include "EventSystem/Event.h"

namespace fw {
CameraComponent::CameraComponent()
{
}

CameraComponent::CameraComponent(vec2 projScale, ViewType type)
    : Component()
{
    m_RequestedProjectionScale = projScale;
    m_CameraView = type;
}

CameraComponent::~CameraComponent()
{
    m_pGameObject->GetScene()->GetGameCore()->GetEventManager()->UnregisterEventListener(this, InputEvent::GetStaticEventType());
}

void CameraComponent::Init()
{
    m_pGameObject->GetScene()->GetGameCore()->GetEventManager()->RegisterEventListener(this, InputEvent::GetStaticEventType());

    AdjustForAspect();
}

void CameraComponent::Update(float deltaTime)
{
    if( GetGameObject()->GetTransform() == nullptr )
        return;

    // Camera is attached to the gameobject that added it. Will move with the object containing it.
    m_Position = GetGameObject()->GetTransform()->GetPosition();
    m_Rotation = GetGameObject()->GetTransform()->GetRotation();

    if (m_RequestedProjectionScale != m_ProjectionScale)
    {
        AdjustForAspect();
    }

    if( m_pGameObject->GetScene()->GetGameCore()->GetFramework()->IsKeyDown( VK_SHIFT ) )
    {
        FirstPersonCamControls(deltaTime);
    }
}

void CameraComponent::Save(WriterType& writer)
{
    Component::Save(writer);

    JSONSaveVec2(writer, "ProjectionScale", m_RequestedProjectionScale);
    JSONSaveInt(writer, "CameraView", static_cast<int>(m_CameraView));
}

void CameraComponent::Load(rapidjson::Value& component)
{
    JSONLoadVec2(component, "ProjectionScale", &m_RequestedProjectionScale);
    int type = 0;
    JSONLoadInt(component, "CameraView", &type);
    m_CameraView = static_cast<ViewType>(type);
}

void CameraComponent::AddToInspector()
{
    //ImGui::Text("%s Camera", GetGameObject()->GetName().c_str());
    ImGui::SliderFloat2("Proj Scale", &m_RequestedProjectionScale.x, 1.0f, 50.0f, "%.2f");

    bool isOrtho = (m_CameraView == ViewType::ORTHOGRAPHIC);
    if( ImGui::Checkbox("Type", &isOrtho) )
    {
        isOrtho == true ? m_CameraView = ViewType::ORTHOGRAPHIC : m_CameraView = ViewType::PERSPECTIVE;
    }
}

void CameraComponent::AdjustForAspect()
{
    m_ProjectionScale = m_RequestedProjectionScale;

    // Sets the window's size based on monitor's width and height.
    int w = m_pGameObject->GetScene()->GetGameCore()->GetFramework()->GetWindowWidth();
    int h = m_pGameObject->GetScene()->GetGameCore()->GetFramework()->GetWindowHeight();
    glViewport(0, 0, w, h);

    // Alter the desired projection to account for aspect ratio for the width.
    m_ProjectionScale.x *= (float)w / h;
    //m_ProjectionScale.y *= (float)h / w;
}

MyMatrix* CameraComponent::GetViewMatrix()
{
    m_ViewMatrix.CreateSRT(1, m_Rotation, m_Position);
    m_ViewMatrix.Inverse();

    return &m_ViewMatrix;
}

MyMatrix* CameraComponent::GetProjectionMatrix()
{
    // Check the CameraComponent's ViewType and determine what kind of projection matrix to create.
    if (GetCameraType() == CameraComponent::ViewType::ORTHOGRAPHIC)
    {
        m_ProjMatrix.CreateOrtho(-GetProjectionScale().x, GetProjectionScale().x,
                                 -GetProjectionScale().y, GetProjectionScale().y,
                                 -1000, 1000);
    }
    else
    {
        m_ProjMatrix.CreatePerspectiveVFoV(45.0f, 1280 / 720.0f, 0.01f, 1000.0f);
    }

    return &m_ProjMatrix;
}

void CameraComponent::FirstPersonCamControls(float deltaTime)
{
    float speed = 10.0f; // World units / second
    float rotSpeed = 120.0f; // Degrees / second

    vec3 position = GetGameObject()->GetTransform()->GetPosition();
    vec3 rotation = GetGameObject()->GetTransform()->GetRotation();

    rotation.y += m_RotationAxes.x * rotSpeed * deltaTime;
    rotation.x += m_RotationAxes.y * rotSpeed * deltaTime;

    MyMatrix rot;
    rot.CreateRotation(m_Rotation);

    position += rot.GetRight() * m_MovementAxes.x * speed * deltaTime;
    position += rot.GetUp() * m_MovementAxes.y * speed * deltaTime;
    position += rot.GetAt() * m_MovementAxes.z * speed * deltaTime;

    GetGameObject()->GetTransform()->SetPosition(position);
    GetGameObject()->GetTransform()->SetRotation(rotation);
}

void CameraComponent::OnEvent(Event* pEvent)
{
    if (pEvent->GetType() == InputEvent::GetStaticEventType())
    {
        InputEvent* pInputEvent = static_cast<InputEvent*>(pEvent);

        if (pInputEvent->GetDeviceType() == InputEvent::DeviceType::Keyboard)
        {
            if (pInputEvent->GetDeviceState() == InputEvent::DeviceState::Pressed)
            {
                if (pInputEvent->GetKeyCode() == 'W') { m_MovementAxes.z += 1; }
                if (pInputEvent->GetKeyCode() == 'S') { m_MovementAxes.z += -1; }
                if (pInputEvent->GetKeyCode() == 'A') { m_MovementAxes.x += -1; }
                if (pInputEvent->GetKeyCode() == 'D') { m_MovementAxes.x += 1; }

                if (pInputEvent->GetKeyCode() == 'Q') { m_MovementAxes.y += 1; }
                if (pInputEvent->GetKeyCode() == 'Z') { m_MovementAxes.y += -1; }

                if (pInputEvent->GetKeyCode() == 'I') { m_RotationAxes.y += 1; }
                if (pInputEvent->GetKeyCode() == 'K') { m_RotationAxes.y += -1; }
                if (pInputEvent->GetKeyCode() == 'J') { m_RotationAxes.x += 1; }
                if (pInputEvent->GetKeyCode() == 'L') { m_RotationAxes.x += -1; }
            }

            if (pInputEvent->GetDeviceState() == InputEvent::DeviceState::Released)
            {
                if (pInputEvent->GetKeyCode() == 'W') { m_MovementAxes.z -= 1; }
                if (pInputEvent->GetKeyCode() == 'S') { m_MovementAxes.z -= -1; }
                if (pInputEvent->GetKeyCode() == 'A') { m_MovementAxes.x -= -1; }
                if (pInputEvent->GetKeyCode() == 'D') { m_MovementAxes.x -= 1; }

                if (pInputEvent->GetKeyCode() == 'Q') { m_MovementAxes.y -= 1; }
                if (pInputEvent->GetKeyCode() == 'Z') { m_MovementAxes.y -= -1; }

                if (pInputEvent->GetKeyCode() == 'I') { m_RotationAxes.y -= 1; }
                if (pInputEvent->GetKeyCode() == 'K') { m_RotationAxes.y -= -1; }
                if (pInputEvent->GetKeyCode() == 'J') { m_RotationAxes.x -= 1; }
                if (pInputEvent->GetKeyCode() == 'L') { m_RotationAxes.x -= -1; }
            }
        }
    }
}

vec2 CameraComponent::GetWorldSpaceMousePosition()
{
    fw::FWCore* pFramework = m_pGameObject->GetScene()->GetGameCore()->GetFramework();

    // Get Window Width and Height for Mouse Positioning.
    int windowWidth = pFramework->GetWindowWidth();
    int windowHeight = pFramework->GetWindowHeight();

    HWND hwnd = pFramework->GetHWND();
    POINT cursorPosition;
    GetCursorPos(&cursorPosition);
    ScreenToClient(hwnd, &cursorPosition);

    // Screen space mouse position.
    float screenX = (float)cursorPosition.x;
    float screenY = (float)windowHeight - cursorPosition.y;

    // Clip space mouse position.
    float clipX = (screenX / windowWidth) * 2 - 1;
    float clipY = (screenY / windowHeight) * 2 - 1;

    // View space mouse position.
    float viewX = clipX * m_ProjectionScale.x;
    float viewY = clipY * m_ProjectionScale.y;

    // World space mouse position.
    float worldX = viewX + m_Position.x;
    float worldY = viewY + m_Position.y;

    // Display Mouse Info 
    //ImGui::Text("Mouse: %0.2f, %0.2f", worldX, worldY);

    return vec2(worldX, worldY);
}

} // namespace fw
