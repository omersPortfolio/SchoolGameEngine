#pragma once

#include "../EventSystem/EventListener.h"

class PlayerController;

namespace fw {
class ComponentRegistry;
class ComponentManager;
class GameCore;
class GameObject;
class Event;
class Camera;
class PhysicsWorld;
class LuaGameState;

class Scene : public EventListener
{
public:
    Scene(GameCore* pGameCore);
    virtual ~Scene();

    virtual void Init();
    virtual void StartFrame(float deltaTime) = 0;
    virtual void OnEvent(Event* pEvent);
    virtual void Update(float deltaTime);
    virtual void Draw();
    virtual void DrawObjectList();
    virtual void DrawNewObjectButtons();
    virtual void DrawImguiDemoWindow();

    virtual void DrawImGuiInspector();
    virtual void Save(const char* filename);

    // Getters.
    inline const char* GetSceneName() { return m_SceneName; }
    GameObject* GetSelectedGameObject() {return m_ImGuiSelectedObject;}
    Camera* GetSceneCamera() {return m_pCamera;}
    GameCore* GetGameCore() { return m_pGameCore; }
    ComponentManager* GetComponentManager() { return m_pComponentManager; }
    std::string GetName() { return m_Name; }
    GameObject* GetGameObject(std::string type);
    std::vector<GameObject*>* GetAllGameObjects() { return &m_Objects; }
    PhysicsWorld* GetWorld() { return m_pWorld; }
    LuaGameState* GetLuaState() { return m_pLuaGameState; }

    bool GetIsObjectListOn() { return m_IsObjectListOn; }

    // Setters.
    void ToggleObjectList() { m_IsObjectListOn = !m_IsObjectListOn; }
    void ToggleObjectDetails() { m_IsObjectInspectorOn = !m_IsObjectInspectorOn; }
    void ToggleImguiDemo() { m_IsImguiDemoWindowOn = !m_IsImguiDemoWindowOn; }

    void CheckAABBCollision();
    void ClearScene();

protected:
    GameCore* m_pGameCore = nullptr;

    const char* m_SceneName = "";

    std::vector<GameObject*> m_Objects;

    Camera* m_pCamera = nullptr;

    ComponentManager* m_pComponentManager = nullptr;
    ComponentRegistry* m_pComponentRegistry = nullptr;

    std::string m_Name = "Scene";

    GameObject* m_ImGuiSelectedObject = nullptr;
    PlayerController* m_pPlayerController = nullptr;

    PhysicsWorld* m_pWorld = nullptr;

    LuaGameState* m_pLuaGameState = nullptr;

    inline static bool m_IsObjectListOn = true;        // bootleg code to fix windows 
    inline static bool m_IsObjectSelectorOn = true;    // reappearing when switching scenes
    inline static bool m_IsObjectInspectorOn = true;
    inline static bool m_IsImguiDemoWindowOn = false;
};

} // namespace fw
