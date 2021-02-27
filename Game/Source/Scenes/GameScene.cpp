#include "GamePCH.h"

#include "GameScene.h"
#include "Game.h"
#include "Objects/Player.h"
#include "Events/GameEvents.h"
#include "Objects/PlayerController.h"

using namespace rapidjson;

GameScene::GameScene(fw::GameCore* pGameCore) :
    Scene(pGameCore)
{
    m_Name = "Game";
}

GameScene::~GameScene()
{
    delete m_Particles;
}

void GameScene::Init()
{
    Scene::Init();

    Game* pGame = GetGame();

    m_SceneName = "Jimmy's Scene";
    //LOG(INFO, "%s has been loaded...", GetSceneName());

    // Create some GameObjects.
    Player* pPlayer = nullptr;
    {
        pPlayer = new Player(this, pGame->GetPlayerController(), "Player", vec2(5, 5), 0, 1,
                             pGame->GetMaterial("Orange"));
        pPlayer->AddComponent(new fw::MeshComponent(pGame->GetMesh("Player")));
        pPlayer->CreatePhysicsBodyBox(fw::BodyType::DynamicBody, 1, 1, 1);
        m_Objects.push_back(pPlayer);
    }

    {
        fw::GameObject* pObject = new fw::GameObject(this, "BGThing", vec2(5, 1), 0, 1,
                                                     pGame->GetMaterial("Water"));
        pObject->AddComponent(new fw::MeshComponent(pGame->GetMesh("Cube")));
        pObject->CreatePhysicsBodyBox(fw::BodyType::StaticBody, 5, 1);
        m_Objects.push_back(pObject);
    }

    m_pCamera = new fw::Camera(this, vec2(5, 5), vec2(2, 2), vec2(5, 5));
    m_pCamera->SetObjectWeAreFollowing(pPlayer);

    m_Particles = new fw::ParticleEmitter(pGame->GetMaterial("Water"));
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

    CheckAABBCollision();

    m_Particles->Update(deltaTime);
    if (m_pCamera != nullptr)
    {
        m_pCamera->Update(deltaTime);
    }

    if (ImGui::Button("Load"))
    {
        Load("Data/SaveFiles/test.scene");
    }
}

void GameScene::Draw()
{
    if (m_pCamera == nullptr)
        return;

    Scene::Draw();

    m_Particles->Draw(m_pCamera);
}

void GameScene::Load(const char* filename)
{
    ClearScene();

    const char* file = fw::LoadCompleteFile(filename, nullptr);
    Game* pGame = static_cast<Game*>(m_pGameCore);

    rapidjson::Document loadedScene;
    loadedScene.Parse(file);
    delete[] file;

    for (rapidjson::Value& gameObject : loadedScene["GameObjects"].GetArray())
    {
        std::string itemName = gameObject["Name"].GetString();

        vec2 itemPosition(0, 0);
        if (gameObject.HasMember("Pos"))
        {
            itemPosition.x = (float)gameObject["Pos"].GetArray()[0].GetDouble();
            itemPosition.y = (float)gameObject["Pos"].GetArray()[1].GetDouble();
        }

        float itemRotation = 0;
        if (gameObject.HasMember("Rot"))
        {
            (float)gameObject["Rot"].GetArray()[0].GetDouble();
        }

        vec2 itemScale(0, 0);
        if (gameObject.HasMember("Scale"))
        {
            itemScale.x = (float)gameObject["Scale"].GetArray()[0].GetDouble();
            itemScale.y = (float)gameObject["Scale"].GetArray()[1].GetDouble();
        }

        if (itemName == "Camera")
        {
            fw::Camera* pCamera = new fw::Camera(this, itemPosition, itemRotation, vec2(5, 5));
            pCamera->SetPosition(itemPosition);

            delete m_pCamera;
            m_pCamera = pCamera;

            continue;
        }

        std::string itemTexture;
        std::string itemShader;
        if (gameObject.HasMember("Textures"))
            itemTexture = gameObject["Textures"].GetString();
        if (gameObject.HasMember("Shader"))
            itemShader = gameObject["Shader"].GetString();

        Color itemColor;
        if (gameObject.HasMember("Color"))
        {
            itemColor.r = (float)gameObject["Color"].GetArray()[0].GetDouble();
            itemColor.g = (float)gameObject["Color"].GetArray()[1].GetDouble();
            itemColor.b = (float)gameObject["Color"].GetArray()[2].GetDouble();
            itemColor.a = (float)gameObject["Color"].GetArray()[3].GetDouble();
        }

        vec2 itemUVScale;
        if (gameObject.HasMember("uvScale"))
        {
            itemUVScale.x = (float)gameObject["uvScale"].GetArray()[0].GetDouble();
            itemUVScale.y = (float)gameObject["uvScale"].GetArray()[1].GetDouble();
        }

        vec2 itemUVOffset;
        if (gameObject.HasMember("uvOffset"))
        {
            itemUVOffset.x = (float)gameObject["uvOffset"].GetArray()[0].GetDouble();
            itemUVOffset.y = (float)gameObject["uvOffset"].GetArray()[1].GetDouble();
        }

        if (itemName == "Player")
        {
            Player* pPlayer = new Player(this, pGame->GetPlayerController(), itemName,
                itemPosition, itemRotation, itemScale, pGame->GetMaterial("Orange"));
            pPlayer->AddComponent(new fw::MeshComponent(pGame->GetMesh("Player")));

            pPlayer->CreatePhysicsBodyBox(fw::BodyType::DynamicBody, 1, 1, 1);
            m_Objects.push_back(pPlayer);
        }
        else
        {
            fw::GameObject* pGameObject = new fw::GameObject(this, itemName,
                itemPosition, itemRotation, itemScale, pGame->GetMaterial("Orange"));

            pGameObject->AddComponent(new fw::MeshComponent(pGame->GetMesh("Player")));
            //pGameObject->AddComponent(new fw::AABBComponent());

            m_Objects.push_back(pGameObject);
            pGameObject->CreatePhysicsBodyBox(fw::BodyType::StaticBody, itemPosition.x, itemPosition.y);
        }
    }
}

Game* GameScene::GetGame()
{
    return static_cast<Game*>(m_pGameCore);
}

void GameScene::DrawNewObjectButtons()
{
    Game* pGame = static_cast<Game*>(m_pGameCore);

    //Changes Scene Name
    ImGui::Text("Rename Scene:");

    std::string clipText = (ImGui::GetClipboardText() != nullptr) ? ImGui::GetClipboardText() : "";
    size_t clipSize = clipText.length();
    //Size of string
    const size_t size = 32;
    char newText[size];
    strncpy_s(newText, size, m_Name.c_str(), sizeof(newText));

    ImGui::InputText("", newText, size);
    SetName(newText);

    ImGui::Separator();

    ImGui::Text("Add to Scene:");

    if (ImGui::Button("GameObject"))
    {
        std::string name = "GameObject(" + std::to_string(m_Objects.size()) + ")";
        fw::GameObject* pObject = new fw::GameObject(this, name, vec2(3, 8), 0, 1,
            pGame->GetMaterial("Orange"));

        pObject->AddComponent(new fw::MeshComponent(pGame->GetMesh("Player")));
        pObject->CreatePhysicsBodyBox(fw::BodyType::StaticBody, 4, 2);

        // Checks if the creation was successful.
        if (pObject)
        {
            m_Objects.push_back(pObject);
            LOG(INFO, "GameObject: %s successfully added to Scene", pObject->GetName().c_str());
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Camera"))
    {
        fw::Camera* pCamera = new fw::Camera(this, vec2(5, 5), vec2(2, 2), vec2(5, 5));

        if (m_pCamera)
        {
            delete m_pCamera;
            LOG(INFO, "Camera: %s successfully added to Scene_%s", pCamera->GetName().c_str(), m_Name.c_str());
        }

        m_pCamera = pCamera;
    }

    ImGui::SameLine();
    if (ImGui::Button("Player"))
    {
        Player* pPlayer = new Player(this, pGame->GetPlayerController(), "Player", 0, 0, 1,
            pGame->GetMaterial("Orange"));

        pPlayer->AddComponent(new fw::MeshComponent(pGame->GetMesh("Player")));
        pPlayer->CreatePhysicsBodyBox(fw::BodyType::DynamicBody, 1, 1, 1);

        //Checks if the creation was successful
        if (pPlayer)
        {
            m_Objects.push_back(pPlayer);
            LOG(INFO, "Player: %s successfully added to Scene", pPlayer->GetName().c_str());
        }
    }
}

void GameScene::SetName(std::string NewName)
{
    m_Name = NewName;
}