#include "FrameworkPCH.h"

#include "Objects/Camera.h"
#include "Scene/Scene.h"
#include "Core/GameCore.h"
#include "Core/FWCore.h"
#include "EventSystem/Event.h"
#include "EventSystem/EventManager.h"

using namespace rapidjson;

namespace fw {

Camera::Camera(Scene* pScene, vec3 pos, vec3 rot, vec2 projScale)
    : GameObject(pScene, "Camera", pos, rot, 1, nullptr)
    , m_RequestedProjectionScale(projScale)
{
    m_Scale = vec3(1, 1, 1);

    m_pScene->GetGameCore()->GetEventManager()->RegisterEventListener(this, InputEvent::GetStaticEventType());
}

Camera::~Camera()
{
    m_pScene->GetGameCore()->GetEventManager()->UnregisterEventListener(this, InputEvent::GetStaticEventType());
}

void Camera::Update(float deltaTime)
{
    if (m_pObjectWeAreFollowing)
    {
        m_Position = m_pObjectWeAreFollowing->GetPosition();
        m_ViewTransform.CreateLookAtView(GetPosition() - vec3(0, 0, 5), vec3(0, 1, 0), GetPosition());
    }
    else
    {
        float speed = 10.0f; // Units / seconds.
        float rotationSpeed = 180.0f; // Degrees / second.

        m_Rotation.x += m_RotationAxes.y * rotationSpeed * deltaTime; // Pitch.
        m_Rotation.y += m_RotationAxes.x * rotationSpeed * deltaTime; // Yaw.

        MyMatrix rot;
        rot.CreateRotation( m_Rotation );

        m_Position += rot.GetRight() * m_MovementAxes.x * speed * deltaTime;
        m_Position += rot.GetUp() * m_MovementAxes.y * speed * deltaTime;
        m_Position += rot.GetAt() * m_MovementAxes.z * speed * deltaTime;

        m_ViewTransform.CreateSRT(1, m_Rotation, m_Position);
        m_ViewTransform.Inverse();
    }

    m_ProjectionScale = m_RequestedProjectionScale;

    int w = m_pScene->GetGameCore()->GetFramework()->GetWindowWidth();
    int h = m_pScene->GetGameCore()->GetFramework()->GetWindowHeight();

    glViewport(0, 0, w, h);

    m_ProjectionScale.x *= (float)w / h;
    //m_ProjectionScale.y *= (float)h / w;
}

void Camera::Save(WriterType& writer)
{
    GameObject::Save(writer);

    // TODO: Save these.
    //vec2 m_RequestedProjectionScale;
    //GameObject* m_pObjectWeAreFollowing = nullptr;
}

void Camera::ImGuiInspector()
{
    GameObject::ImGuiInspector();

    ImGui::DragFloat2("Projection Scale", &m_ProjectionScale.x);

    ImGui::NewLine();

    if (m_pObjectWeAreFollowing != nullptr)
    {
        ImGui::Text("Object Following: %s", m_pObjectWeAreFollowing->GetName().c_str());
    }
    else
    {
        ImGui::Text("No Object To Follow");
    }
}

void Camera::OnEvent(Event* pEvent)
{
    if (pEvent->GetType() == InputEvent::GetStaticEventType())
    {
        InputEvent* pInputEvent = static_cast<InputEvent*>(pEvent);

        if (pInputEvent->GetDeviceType() == InputEvent::DeviceType::Keyboard)
        {
            if (pInputEvent->GetDeviceState() == InputEvent::DeviceState::Pressed)
            {
                if (pInputEvent->GetKeyCode() == 'W') { m_MovementAxes.z +=  1; }
                if (pInputEvent->GetKeyCode() == 'S') { m_MovementAxes.z += -1; }
                if (pInputEvent->GetKeyCode() == 'D') { m_MovementAxes.x +=  1; }
                if (pInputEvent->GetKeyCode() == 'A') { m_MovementAxes.x += -1; }
                if (pInputEvent->GetKeyCode() == 'Q') { m_MovementAxes.y +=  1; }
                if (pInputEvent->GetKeyCode() == 'Z') { m_MovementAxes.y += -1; }

                if (pInputEvent->GetKeyCode() == 'I') { m_RotationAxes.y +=  1; }
                if (pInputEvent->GetKeyCode() == 'K') { m_RotationAxes.y += -1; }
                if (pInputEvent->GetKeyCode() == 'J') { m_RotationAxes.x +=  1; }
                if (pInputEvent->GetKeyCode() == 'L') { m_RotationAxes.x += -1; }
            }

            if (pInputEvent->GetDeviceState() == InputEvent::DeviceState::Released)
            {
                if (pInputEvent->GetKeyCode() == 'W') { m_MovementAxes.z -=  1; }
                if (pInputEvent->GetKeyCode() == 'S') { m_MovementAxes.z -= -1; }
                if (pInputEvent->GetKeyCode() == 'D') { m_MovementAxes.x -=  1; }
                if (pInputEvent->GetKeyCode() == 'A') { m_MovementAxes.x -= -1; }
                if (pInputEvent->GetKeyCode() == 'Q') { m_MovementAxes.y -=  1; }
                if (pInputEvent->GetKeyCode() == 'Z') { m_MovementAxes.y -= -1; }

                if (pInputEvent->GetKeyCode() == 'I') { m_RotationAxes.y -=  1; }
                if (pInputEvent->GetKeyCode() == 'K') { m_RotationAxes.y -= -1; }
                if (pInputEvent->GetKeyCode() == 'J') { m_RotationAxes.x -=  1; }
                if (pInputEvent->GetKeyCode() == 'L') { m_RotationAxes.x -= -1; }
            }
        }
    }
}

vec2 Camera::GetWorldSpaceMousePosition()
{
    fw::FWCore* pFramework = m_pScene->GetGameCore()->GetFramework();

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

    //Mouse Debug
    //ImGui::Text("Mouse Screen: %0.2f, %0.2f", screenX, screenY);
    //ImGui::Text("Mouse Clip:   %0.2f, %0.2f", clipX, clipY);
    //ImGui::Text("Mouse View:   %0.2f, %0.2f", viewX, viewY);
    ImGui::Text("Mouse: %0.2f, %0.2f", worldX, worldY);

    return vec2(worldX, worldY);
}

} // namespace fw
