#pragma once

#include "Objects/GameObject.h"
#include "Math/MyMatrix.h"

namespace fw {

class Camera : public GameObject
{
public:
    Camera(Scene* pScene, vec3 pos, vec3 rot, vec2 projScale);
    virtual ~Camera();

    virtual void Update(float deltaTime) override;
    virtual void ImGuiInspector() override;
    virtual void OnEvent(Event* pEvent) override;

    // Getters.
    vec2 GetProjectionScale() { return m_ProjectionScale; }
    vec2 GetWorldSpaceMousePosition();
    const MyMatrix& GetViewTransform() { return m_ViewTransform; }

    // Setters.
    void SetObjectWeAreFollowing(GameObject* pObj) { m_pObjectWeAreFollowing = pObj; }
    virtual void Save(WriterType& writer) override;

protected:
    vec2 m_RequestedProjectionScale;
    vec2 m_ProjectionScale;
    GameObject* m_pObjectWeAreFollowing = nullptr;

    vec3 m_MovementAxes = vec3(0); // X = left/right, Y = up/down, Z = in/out
    vec2 m_RotationAxes = vec2(0); // X = Yaw, Y = Pitch

    MyMatrix m_ViewTransform;
};

} // namespace fw
