#pragma once

#include "../EventSystem/EventListener.h"

class PlayerController;

namespace fw {

class ComponentManager;
class CameraComponent;
class Event;
class GameCore;
class GameObject;
class LuaGameState;
class Material;
class PhysicsWorld;
class ResourceManager;

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
    virtual void DrawMousePickingMeshes();
    virtual void DrawObjectList();
    bool DrawObject(GameObject* pObject, int& item_selected, int& index);
    virtual void DrawNewObjectButtons();
    virtual void DrawImguiDemoWindow();
    virtual void DrawImGuiInspector();
    
    // Save/Load.
    void LoadFromFile(std::string path, std::string sceneName);
    void SaveToFile();
    void SaveToFile(std::string filename);

    // Getters.
    std::string GetPath() { return m_Path; }
    GameObject* GetSelectedGameObject() {return m_ImGuiSelectedObject;}
    GameCore* GetGameCore() { return m_pGameCore; }
    ComponentManager* GetComponentManager() { return m_pComponentManager; }
    std::string GetName() { return m_Name; }
    GameObject* GetGameObject(std::string type);
    std::vector<GameObject*>* GetAllGameObjects() { return &m_Objects; }
    PhysicsWorld* GetWorld() { return m_pWorld; }
    LuaGameState* GetLuaState() { return m_pLuaGameState; }

    bool GetIsObjectListOn() { return m_IsObjectListOn; }

    CameraComponent* GetActiveCameraComponent() { return m_pActiveCameraComponent; }

    // Setters.
    virtual void SetPath(std::string path) { m_Path = path; }
    virtual void SetName(std::string name) { m_Name = name; }

    virtual void SelectObject(int selectedObject);
    void ToggleObjectList() { m_IsObjectListOn = !m_IsObjectListOn; }
    void ToggleObjectDetails() { m_IsObjectInspectorOn = !m_IsObjectInspectorOn; }
    void ToggleImguiDemo() { m_IsImguiDemoWindowOn = !m_IsImguiDemoWindowOn; }

    void CheckAABBCollision();
    void ClearScene();

    void SetActiveCameraComponent(CameraComponent* pActiveCamera) { m_pActiveCameraComponent = pActiveCamera; }
    bool m_DebugDraw = true;

protected:
    std::string m_Path = "";
    std::string m_Name = "Scene";

    GameCore* m_pGameCore = nullptr;
    std::vector<GameObject*> m_Objects;

    ComponentManager* m_pComponentManager = nullptr;
    CameraComponent* m_pActiveCameraComponent = nullptr;

    GameObject* m_ImGuiSelectedObject = nullptr;
    PlayerController* m_pPlayerController = nullptr;
    PhysicsWorld* m_pWorld = nullptr;

    LuaGameState* m_pLuaGameState = nullptr;

    // For mouse picking.
    Material* m_pMousePickerMaterial = nullptr;

    // bootleg code to fix windows reappearing when switching scenes.
    inline static bool m_IsObjectListOn = true;
    inline static bool m_IsObjectSelectorOn = true;
    inline static bool m_IsObjectInspectorOn = true;
    inline static bool m_IsImguiDemoWindowOn = false;
};

} // namespace fw
