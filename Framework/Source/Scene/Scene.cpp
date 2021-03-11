#include "FrameworkPCH.h"

#include "EventSystem/EventManager.h"

#include "Scene.h"
#include "Core/GameCore.h"
#include "Objects/ResourceManager.h"
#include "Objects/GameObject.h"
#include "Objects/Camera.h"
#include "Components/ComponentManager.h"
#include "Components/ComponentRegistry.h"
#include "EventSystem/Event.h"
#include "Log.h"
#include "Components/AABBComponent.h"
#include "Components/MeshComponent.h"
#include "Objects/AABB.h"
#include "Physics/PhysicsWorld.h"
#include "Physics/PhysicsWorld2D.h"
#include "Utility/JSONHelpers.h"
#include "Lua/LuaGameState.h"
#include "Lua/LuaScript.h"

namespace fw {

Scene::Scene(GameCore* pGameCore)
{
    m_pGameCore = pGameCore;

    m_pComponentManager = new ComponentManager();
    m_pComponentRegistry = new ComponentRegistry();

    m_pGameCore->GetEventManager()->RegisterEventListener(this, RemoveFromGameEvent::GetStaticEventType());
}

Scene::~Scene()
{
    for (GameObject* pObject : m_Objects)
    {
        delete pObject;
    }

    delete m_pCamera;

    delete m_pWorld;

    delete m_pComponentManager;
    delete m_pComponentRegistry;

#if FW_USING_LUA
    delete m_pLuaGameState;
#endif

    m_pGameCore->GetEventManager()->UnregisterEventListenerCompletely(this);
}

void Scene::OnEvent(Event* pEvent)
{
    if (pEvent->GetType() == RemoveFromGameEvent::GetStaticEventType())
    {
        RemoveFromGameEvent* pRemoveFromGameEvent = static_cast<RemoveFromGameEvent*>(pEvent);
        GameObject* pObject = pRemoveFromGameEvent->GetGameObject();

        if (pObject == m_pCamera)
        {
            delete m_pCamera;
            m_pCamera = nullptr;
            return;
        }

        auto it = std::find(m_Objects.begin(), m_Objects.end(), pObject);
        m_Objects.erase(it);

        delete pObject;
    }
}

void Scene::Init()
{
    m_pWorld = new PhysicsWorld2D(m_pGameCore->GetEventManager());
    m_pWorld->SetCanSleep(true);
    m_pWorld->SetGravity(vec3(0, -10, 0));
}

void Scene::Update(float deltaTime)
{
    if (m_pWorld)
    {
        m_pWorld->Update(deltaTime);
    }

    for (GameObject* pObject : m_Objects)
    {
        pObject->Update(deltaTime);
    }

    GetComponentManager()->Update(deltaTime);

    if (m_pCamera)
    {
        vec2 mousePos = m_pCamera->GetWorldSpaceMousePosition();
    }
}

void Scene::Draw()
{
    if (m_pCamera == nullptr)
        return;

    GetComponentManager()->Draw(m_pCamera);
}

void Scene::ClearScene()
{
    m_ImGuiSelectedObject = nullptr;

    for (size_t i = 0; i < m_Objects.size(); i++)
    {
        delete m_Objects[i];
    }
    m_Objects.clear();
}

GameObject* Scene::GetGameObject(std::string type)
{
    for (auto& object : m_Objects)
    {
        if (object->GetName() == type)
            return object;
    }

    LOG(ERROR, "GameObject not found");
    return nullptr;
}

void Scene::DrawObjectList()
{
    if (m_IsObjectListOn == true)
    {
        ImGui::Begin("Object List", &m_IsObjectListOn);

        // Sets the color of the Scene Name at the top of the list.
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Scene: %s", GetName().c_str());
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Right-click for Options");

        // Opens a popup menu when Name is right-clicked.
        if (ImGui::BeginPopupContextItem("Object Selection Menu"))
        {
            // Displays list of buttons to add objects, located in Child Scene.
            DrawNewObjectButtons();
            ImGui::EndPopup();
        }

        ImGui::Separator();

        ImGui::Text("Objects:");

        // Arranges Objects vector into a selectable list.
        static int item_current_idx = 0;
        if (ImGui::BeginListBox("Objects", ImVec2(-1, -1)))
        {
            for (int n = 0; n < m_Objects.size() + 1; n++)
            {
                GameObject* pObject = nullptr;

                if (n == m_Objects.size())
                {
                    if (m_pCamera == nullptr)
                        continue;
                    pObject = m_pCamera;
                }
                else
                {
                    pObject = m_Objects[n];
                }

                ImGui::PushID(pObject);

                const bool is_selected = (item_current_idx == n);
                if (ImGui::Selectable(pObject->GetName().c_str(), is_selected))
                    item_current_idx = n;

                // Pops up a toolbar when hovered over an Object.
                if (ImGui::IsItemHovered())
                {
                    //ImGui::SetTooltip("Right-click for Options");

                    //Opens Object Inspector when Object is clicked
                    if (ImGui::IsMouseDown(0))
                    {
                        m_ImGuiSelectedObject = pObject;
                        m_IsObjectInspectorOn = true;
                    }
                }

                // Opens a popup menu when Name is right-clicked.
                if (ImGui::BeginPopupContextItem("Object Options"))
                {
                    // Changes Object Name.
                    ImGui::Text("Rename:");

                    std::string clipText = (ImGui::GetClipboardText() != nullptr) ? ImGui::GetClipboardText() : "";
                    size_t clipSize = clipText.length();
                    const size_t size = 32;
                    char newText[size];
                    strncpy_s(newText, size, pObject->GetName().c_str(), sizeof(newText));

                    ImGui::InputText("", newText, size);
                    pObject->SetName(newText);

                    ImGui::Separator();

                    if (ImGui::Button("Duplicate"))
                    {
                        GameObject* newObj = new GameObject(*pObject);

                        m_Objects.push_back(newObj);
                    }

                    ImGui::Separator();

                    // Deletes Object.
                    if (ImGui::Button("Delete Object"))
                    {
                        m_pGameCore->GetEventManager()->AddEvent(new RemoveFromGameEvent(pObject));
                        m_ImGuiSelectedObject = nullptr;

                        LOG(INFO, "%s successfully deleted from Scene", pObject->GetName().c_str());
                    }

                    ImGui::Separator();

                    // List of Components to Add
                    // UI for adding components to game objects
                    if (ImGui::CollapsingHeader("Add Components"))
                    {
                        m_pComponentRegistry->AddInterfaceToCreateComponents( pObject );
                    }

                    ImGui::EndPopup();
                }
                ImGui::PopID();
            }
            ImGui::EndListBox();
        }
        ImGui::End();
    }
}

void Scene::DrawNewObjectButtons()
{
}

void Scene::DrawImguiDemoWindow()
{
    if (m_IsImguiDemoWindowOn == true)
    {
        ImGui::ShowDemoWindow();
    }
}

void Scene::Save(const char* filename)
{
    rapidjson::StringBuffer buffer;
    WriterType writer(buffer);
    writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);

    writer.StartObject();
    {
        writer.String("GameObjects");
        writer.StartArray();

        writer.StartObject();
        {
            m_pCamera->Save(writer);
            writer.EndObject();
        }

        for (fw::GameObject* obj : m_Objects)
        {
            writer.StartObject();
            obj->Save(writer);
            writer.EndObject();
        }

        writer.EndArray();
        writer.EndObject();
    }

    // Save json to disk.
    FILE* pFile = nullptr;
    fopen_s(&pFile, filename, "wb");
    if (pFile != nullptr)
    {
        const char* str = buffer.GetString();
        int length = (int)strlen(str);
        fwrite(str, 1, length, pFile);
        fclose(pFile);
    }
}

void Scene::CheckAABBCollision()
{
    std::vector<fw::Component*> pAABBComponents = GetComponentManager()->GetComponentList(AABBComponent::GetStaticType());

    for (int i = 0; i < pAABBComponents.size(); i++)
    {
        AABBComponent* pComponentI = static_cast<AABBComponent*>(pAABBComponents[i]);

        for (int j = i + 1; j < pAABBComponents.size(); j++)
        {
            AABBComponent* pComponentJ = static_cast<AABBComponent*>(pAABBComponents[j]);

            if (pComponentI->GetAABB().CheckCollision(pComponentJ->GetAABB()))
            {
                GetGameCore()->GetEventManager()->AddEvent(
                    new AABBCollisionEvent(pComponentI->GetGameObject(),
                    pComponentJ->GetGameObject()));
            }
        }
    }
}

void Scene::DrawImGuiInspector()
{
    if (m_IsObjectInspectorOn == true)
    {
        ImGui::Begin("Inspector", &m_IsObjectInspectorOn);

        if (m_ImGuiSelectedObject != nullptr)
        {
            m_ImGuiSelectedObject->ImGuiInspector();
        }
        else
        {
            ImGui::Text("Select an object to inspect...");
        }

        ImGui::End();
    }
}

} // namespace fw
