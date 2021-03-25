#include "GamePCH.h"

#include "GameScene.h"
#include "Game.h"
#include "Components/DirectMovementComponent.h"
#include "Components/ExampleComponent.h"
#include "Components/AIFollowComponent.h"
#include "Components/PhysicsMovementComponent.h"
#include "Events/GameEvents.h"
#include "Objects/PlayerController.h"

using namespace rapidjson;

GameScene::GameScene(fw::GameCore* pGameCore) :
    Scene(pGameCore)
{
    m_Name = "GameScene";
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
    Scene::Init();

    Game* pGame = GetGame();

    //LOG(INFO, "%s has been loaded...", GetSceneName());

//    // Create some GameObjects.
//    {
//        fw::GameObject* pGameObject = nullptr;
//        pGameObject = new fw::GameObject(this, "Player");
//        pGameObject->AddComponent(new fw::TransformComponent(vec2(5, 5), 0, 1));
//        pGameObject->AddComponent(new fw::MeshComponent(pGame->GetMesh("Sprite"), pGame->GetMaterial("Orange")));
//        pGameObject->AddComponent(new fw::CollisionComponent(vec3(0, 0, 0), fw::ShapeType::Box, fw::BodyType::DynamicBody, vec3(1, 1), 1));
//        pGameObject->AddComponent(new fw::ParticleEmitterComponent(pGame->GetMaterial("Water")));
//
//        //pPlayer->AddComponent(new ExampleComponent());
//
//#if FW_USING_LUA
//        pPlayer->AddComponent(new fw::LuaScriptComponent(GetGameCore()->GetEventManager()));
//#endif
//
//        m_Objects.push_back(pGameObject);
//    }
//
//    {
//        fw::GameObject* pObject = new fw::GameObject(this, "Floor");
//        pObject->AddComponent(new fw::TransformComponent(vec2(5, 1), 0, 1));
//        pObject->AddComponent(new fw::MeshComponent(pGame->GetMesh("Cube"), pGame->GetMaterial("Water")));
//        pObject->AddComponent(new fw::CollisionComponent(vec3(0, 0, 0), fw::ShapeType::Box, fw::BodyType::StaticBody, vec3(5, 1)));
//        m_Objects.push_back(pObject);
//    }
//
//    // Camera.
//    {
//        fw::GameObject* pObject = new fw::GameObject(this, "Camera");
//        fw::CameraComponent* pCamera = new fw::CameraComponent(vec2(5.0f, 5.0f), fw::CameraComponent::ViewType::PERSPECTIVE);
//        pObject->AddComponent(new fw::TransformComponent(vec3(5, 5, -15), 0, 1));
//        pObject->AddComponent(pCamera);
//        m_Objects.push_back(pObject);
//
//        // Set an active camera
//        SetActiveCameraComponent(pCamera);
//    }

#if FW_USING_LUA
    m_pLuaGameState = new fw::LuaGameState();
#endif
}

void GameScene::StartFrame(float deltaTime)
{
    //Scene::StartFrame( deltaTime ); // Pure virtual in base class.
}

void GameScene::OnEvent(fw::Event* pEvent)
{
    Scene::OnEvent(pEvent);

    if (pEvent->GetType() == fw::AABBCollisionEvent::GetStaticEventType())
    {
        fw::AABBCollisionEvent* pAABBCollisionEvent = static_cast<fw::AABBCollisionEvent*>(pEvent);
        fw::GameObject* pAObject = pAABBCollisionEvent->GetGameAObject();
        fw::GameObject* pBObject = pAABBCollisionEvent->GetGameBObject();

        ImGui::Begin("AABB OverlapEvent");
        ImGui::Text("Object: %s", pAObject->GetName().c_str());
        ImGui::Text(" - HAS OVERLAPPED With - Object: %s", pBObject->GetName().c_str());
        ImGui::End();
    }
}

void GameScene::Update(float deltaTime)
{
    Scene::Update(deltaTime);

    UpdateExampleComponents(deltaTime);
    UpdateDirectMovementComponents(deltaTime);
    UpdatePhysicsMovementComponents(deltaTime);
    UpdateAIFollowComponents(deltaTime);

    CheckAABBCollision();

    if (m_pActiveCameraComponent != nullptr)
    {
        m_pActiveCameraComponent->Update(deltaTime);
    }
}

void GameScene::UpdateExampleComponents(float deltaTime)
{
    std::vector<fw::Component*>& list = m_pComponentManager->GetComponentList(ExampleComponent::GetStaticType());

    for (int i = 0; i < list.size(); i++)
    {
        ExampleComponent* pComponent = static_cast<ExampleComponent*>(list[i]);
        pComponent->Update(deltaTime);
    }
}

void GameScene::UpdateDirectMovementComponents(float deltaTime)
{
    std::vector<fw::Component*>& list = m_pComponentManager->GetComponentList(DirectMovementComponent::GetStaticType());

    for (int i = 0; i < list.size(); i++)
    {
        DirectMovementComponent* pComponent = static_cast<DirectMovementComponent*>(list[i]);
        pComponent->Update(deltaTime);
    }
}

void GameScene::UpdatePhysicsMovementComponents(float deltaTime)
{
    std::vector<fw::Component*>& list = m_pComponentManager->GetComponentList(PhysicsMovementComponent::GetStaticType());

    for (int i = 0; i < list.size(); i++)
    {
        PhysicsMovementComponent* pComponent = static_cast<PhysicsMovementComponent*>(list[i]);
        pComponent->Update(deltaTime);
    }
}

void GameScene::UpdateAIFollowComponents(float deltaTime)
{
    std::vector<fw::Component*>& list = m_pComponentManager->GetComponentList(AIFollowComponent::GetStaticType());

    for (int i = 0; i < list.size(); i++)
    {
        AIFollowComponent* pComponent = static_cast<AIFollowComponent*>(list[i]);
        pComponent->Update(deltaTime);
    }
}

void GameScene::Draw()
{
    if (m_pActiveCameraComponent == nullptr)
        return;

    Scene::Draw();
}

Game* GameScene::GetGame()
{
    return static_cast<Game*>(m_pGameCore);
}

void GameScene::DrawNewObjectButtons()
{
    Game* pGame = static_cast<Game*>(m_pGameCore);

    ImGui::Text("Add to Scene:");

    fw::GameObject* pObject = nullptr;
    if (ImGui::Button("GameObject"))
    {
        std::string name = "GameObject(" + std::to_string(m_Objects.size()) + ")";
        pObject = new fw::GameObject(this, name); //, vec2(3, 8), 0, 1);
    }

    if (ImGui::Button("GameObject w/ Transform & Mesh"))
    {
        std::string name = "GameObject(" + std::to_string(m_Objects.size()) + ")";
        pObject = new fw::GameObject(this, name); //, vec2(3, 8), 0, 1);

        pObject->AddComponent(new fw::TransformComponent());
        pObject->AddComponent(new fw::MeshComponent(pGame->GetMesh("Sprite"), pGame->GetMaterial("Orange")));
    }

    if (ImGui::Button("GameObject w/ Transform & Camera"))
    {
        std::string name = "Camera(" + std::to_string(m_Objects.size()) + ")";
        pObject = new fw::GameObject(this, name); //, vec2(3, 8), 0, 1);

        pObject->AddComponent(new fw::TransformComponent());
        fw::CameraComponent* pCamera = new fw::CameraComponent(vec2(5,5), fw::CameraComponent::ViewType::PERSPECTIVE);
        pObject->AddComponent(pCamera);

        pObject->GetTransform()->SetPosition( 0, 0, -5 );

        SetActiveCameraComponent( pCamera );
    }

    // Checks if the creation was successful.
    if (pObject)
    {
        m_Objects.push_back(pObject);
        LOG(INFO, "GameObject: %s successfully added to Scene", pObject->GetName().c_str());
    }
}

void GameScene::SetName(std::string NewName)
{
    m_Name = NewName;
}