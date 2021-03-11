#include "GamePCH.h"

#include "Game.h"
#include "Objects/Player.h"
#include "Objects/PlayerController.h"
#include "Objects/Shapes.h"
#include "Events/GameEvents.h"
#include "Scenes/GameScene.h"

Game::Game(fw::FWCore* pFramework) : fw::GameCore(pFramework)
{
    wglSwapInterval(m_VSyncEnabled ? 1 : 0);
}

Game::~Game()
{
    delete m_pFBO;

    delete m_pScene;

    for (std::pair<std::string, fw::AABB*> object : m_pAABB)
    {
        delete object.second;
    }

    delete m_pPlayerController;

    delete m_pResourceManager;

    delete m_pResourcePanel;

    delete m_pEventManager;

    delete m_pImGuiManager;

    delete m_pTextureManager;

    delete m_pTransformGizmo2D;

    delete m_Log;
}

void Game::Init()
{
    // OpenGL Settings.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    // Enable backface culling.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    m_pFBO = new fw::FrameBufferObject(m_pFramework->GetWindowWidth(), m_pFramework->GetWindowHeight());

    m_pEventManager = new fw::EventManager();
    m_pEventManager->RegisterEventListener(this, fw::InputEvent::GetStaticEventType());

    m_pImGuiManager = new fw::ImGuiManager(m_pFramework);
    m_pImGuiManager->Init();
    m_pEventManager->RegisterEventListener(m_pImGuiManager, fw::InputEvent::GetStaticEventType());

    m_pResourceManager = new fw::ResourceManager();
    
    //Enable docking
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // This is just to read and write log files into Log.txt
    // which is handled in Log's constructor and destructor.
    m_Log = new Log();

    // Set up player controller and register it for input events.
    m_pPlayerController = new PlayerController();
    m_pEventManager->RegisterEventListener(m_pPlayerController, fw::InputEvent::GetStaticEventType());

    // Load some Shaders.
    m_pResourceManager->AddShader("Basic", "Data/Shaders/Basic.vert", "Data/Shaders/Basic.frag");
    m_pResourceManager->AddShader("Water", "Data/Shaders/Water.vert", "Data/Shaders/Water.frag");

    // Create some Meshes.
    m_pResourceManager->AddMesh("Player", meshPrimType_Sprite, meshNumVerts_Sprite, meshAttribs_Sprite);
    m_pResourceManager->AddMesh("DynamicMeshPlayer", new fw::Mesh());
    fw::Mesh* tempPlane = new fw::Mesh(vec2(10, 10), ivec2(40, 40));
    m_pResourceManager->AddMesh("Plane", tempPlane);

    fw::Mesh* pCube = new fw::Mesh();
    pCube->CreateFromOBJ( "Data/Meshes/Cube.obj" );
    m_pResourceManager->AddMesh("Cube", pCube);

    // Load some textures... now handled by TextureManager automatically.
    m_pTextureManager = new fw::TextureManager("Data/Textures", m_pResourceManager->GetTexturesMap_TempRemoveMe(), m_pScene);

    // Create some Materials.
    m_pResourceManager->AddMaterial("Orange", new fw::Material(m_pResourceManager->GetShader("Basic"), m_pResourceManager->GetTexture("Orange"), fw::Color::White(), 1, 0));
    m_pResourceManager->AddMaterial("Water", new fw::Material(m_pResourceManager->GetShader("Basic"), m_pResourceManager->GetTexture("Water"), fw::Color::White(), 1, 0));
    m_pResourceManager->AddMaterial("Green", new fw::Material(m_pResourceManager->GetShader("Basic"), m_pResourceManager->GetTexture("Green"), fw::Color::White(), 1, 0));
    // Load our scene.
    m_pScene = new GameScene(this);
    m_pScene->Init();

    m_pResourcePanel = new fw::ResourcesPanel(m_pResourceManager, m_pTextureManager,"Resource Panel", m_pScene);
    //Add Tabs to the Resource Panel
    m_pResourcePanel->AddResource("Materials",  fw::ResourceType::Materials,true,  ImVec4(0.3f, 0.6f, 0.4f, 1.0f));
    m_pResourcePanel->AddResource("Textures",  fw::ResourceType::Textures,true,  ImVec4(0.7f, 0.4f, 1.0f, 1.0f));
    m_pResourcePanel->AddResource("Shaders",   fw::ResourceType::Shaders,true,  ImVec4(0.9f, 0.7f, 0.5f, 1.0f));
    m_pResourcePanel->AddResource("Meshes",    fw::ResourceType::Meshes,true, ImVec4(0.9f, 0.2f, 0.4f, 1.0f));

    ImGui::LoadIniSettingsFromDisk("LayoutDefault.ini");

    m_pTransformGizmo2D = new fw::TransformGizmo2D(this);
    LOG(INFO, "Game Initialized...");
}

void Game::StartFrame(float deltaTime)
{
    // Start the Imgui frame, must come before any other calls to imgui display functions.
    m_pImGuiManager->StartFrame(deltaTime);

    // Reset the controller.
    m_pPlayerController->StartFrame();

    // Process our events.
    m_pEventManager->DispatchAllEvents(deltaTime, this);

    m_pScene->StartFrame(deltaTime);
}

void Game::OnEvent(fw::Event* pEvent)
{
}

void Game::Update(float deltaTime)
{
    m_pScene->Update(deltaTime);
    m_pTransformGizmo2D->Update(m_pScene);

    Editor_SetupDocking();
    Editor_MainMenu();
    
    // Displaying Texture Manager window
    if (m_pTextureManager->GetOnOffState())
    {
        m_pTextureManager->Update(m_pScene);
    }

    if (m_pResourcePanel->GetOnOffState())
    {
        m_pResourcePanel->Update(m_pScene);
    }

    // Draws the Log window if Log window is on
    if (m_LogOpen)
    {
        Log::Update(&m_LogOpen);
    }

    // Debug imgui stuff.
    {
        // Display framerate.
        ImGui::Text("%0.2f", 1 / deltaTime);

        if (ImGui::Checkbox("VSync", &m_VSyncEnabled))
        {
            wglSwapInterval(m_VSyncEnabled ? 1 : 0);
        }

        //ImGui::ShowDemoWindow();
    }
}

void Game::Draw()
{
    m_pFBO->Bind();
    glClearColor(0, 0, 0.2f, 1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_pFBO->GetRequestedWidth(), m_pFBO->GetRequestedHeight());
    m_pTransformGizmo2D->Draw();
    m_pScene->Draw();
    m_pFBO->Unbind();

    glClearColor(0.141f, 0.149f, 0.219f, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_pFramework->GetWindowWidth(), m_pFramework->GetWindowHeight());

    {
        ImGui::Begin("Game");
        {
            ImGui::BeginChild("Render");
            GameRenderWindowPos = ImGui::GetWindowPos();
            GameRenderwindowSize = ImGui::GetWindowSize();
            ImTextureID tex_id = (void*)(intptr_t)m_pFBO->GetColorTextureHandle();        
            ImVec2 size = ImVec2(GameRenderwindowSize.x, GameRenderwindowSize.y);
        
            ImVec2 uvTopLeft(0, m_pFBO->GetHeightRatio());
            ImVec2 uvBottomRight(m_pFBO->GetWidthRatio(), 0);
            ImGui::Image(tex_id, size, uvTopLeft, uvBottomRight);
            ImGui::EndChild();
        }
        ImGui::End();
    }
    
    m_pScene->DrawObjectList();
    m_pScene->DrawImGuiInspector();
    m_pScene->DrawImguiDemoWindow();

    m_pImGuiManager->EndFrame();
}

void Game::Editor_MainMenu()
{
    fw::Scene* pNewScene = nullptr;
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New Scene"))
        {
            pNewScene = new GameScene(this);
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Save"))
        {
            m_pScene->Save("Data/SaveFiles/test.scene");
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Quit"))
        {
            m_pFramework->Shutdown();
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Layout"))
    {
        if (ImGui::MenuItem("Reset"))
        {
            // TODO: When reloaded, throws away all docking settings.
            ImGui::LoadIniSettingsFromDisk("LayoutDefault.ini");
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View"))
    {
        if (ImGui::MenuItem("Log")) { m_LogOpen = !m_LogOpen; }
        if (ImGui::MenuItem(m_pResourcePanel->GetPanelName())) { m_pResourcePanel->ToggleOnOff(); }
        if (ImGui::MenuItem("Texture Manager")) { m_pTextureManager->ToggleOnOff(); }
        if (ImGui::MenuItem("Object List")) { m_pScene->ToggleObjectList(); }
        if (ImGui::MenuItem("Object Inspector")) { m_pScene->ToggleObjectDetails(); }
        if (ImGui::MenuItem("Imgui Helper")) { m_pScene->ToggleImguiDemo(); }
        
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();

    if (pNewScene != nullptr)
    {
        delete m_pScene;
        m_pScene = pNewScene;
        m_pScene->Init();
        m_pScene->Update(0);
    }
}

void Game::Editor_SetupDocking()
{
    bool open = true;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("DefaultLayout", &open, window_flags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspace_id = ImGui::GetID("Layout");
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    ImGui::End();

    // ImGui style. TODO: This should not be done every frame. Move to ImGuiManager?
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
        style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        style.Colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
        style.Colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
        style.Colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
        style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
        style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
        style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
        style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
        style.GrabRounding = style.FrameRounding = 2.3f;
    }
}
