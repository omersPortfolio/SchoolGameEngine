#pragma once

#include "EventSystem/EventListener.h"
#include "Utility/JSONHelpers.h"
#include "Math/Vector.h"

namespace fw {

class GameObject;

class Component : public EventListener
{
public:
    Component() {}
    virtual ~Component() {}

    virtual const char* GetType() = 0;

    virtual void Save(WriterType& writer) {}
    virtual void Load(rapidjson::Value& component) {}

    GameObject* GetGameObject() { return m_pGameObject; }
    void SetGameObject(GameObject* pGameObject) { m_pGameObject = pGameObject; }

    virtual void Init() {};
    virtual void Update(float deltaTime) { };

    virtual void ImGuiInspector() {};

protected:
    GameObject* m_pGameObject = nullptr;
};

} // namespace fw
