#pragma once

class PlayerController;

class Game : public fw::GameCore
{
public:
    Game(fw::FWCore* pFramework);
    virtual ~Game();

    void Init();
    virtual void StartFrame(float deltaTime) override;
    virtual void OnEvent(fw::Event* pEvent) override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

    void GetGameRenderWindowInfo(ImVec2* WindowPos = nullptr, ImVec2* WindowSize = nullptr) override { *WindowSize = GameRenderwindowSize; *WindowPos = GameRenderWindowPos;}
    void Editor_MainMenu();
    void Editor_SetupDocking();
    
    
    fw::ShaderProgram* GetShader(std::string name) { return m_pResourceManager->GetShader(name); }
    fw::Mesh* GetMesh(std::string name) { return m_pResourceManager->GetMesh(name); }
    fw::Texture* GetTexture(std::string name) { return m_pResourceManager->GetTexture(name); }
    fw::Material* GetMaterial(std::string name) { return m_pResourceManager->GetMaterial(name); }

    PlayerController* GetPlayerController() { return m_pPlayerController; }

protected:
    fw::ImGuiManager* m_pImGuiManager = nullptr;

    PlayerController* m_pPlayerController = nullptr;

    fw::FrameBufferObject* m_pFBO = nullptr;

    std::map<std::string, fw::AABB*> m_pAABB;
    fw::Scene* m_pScene = nullptr;

    // Settings.
    bool m_VSyncEnabled = true;
    bool m_LogOpen = true;
    Log* m_Log;

    ImVec2 GameRenderWindowPos;
    ImVec2 GameRenderwindowSize;
    fw::TextureManager* m_pTextureManager;
    fw::ResourcesPanel* m_pResourcePanel;
    fw::TransformGizmo2D* m_pTransformGizmo2D = nullptr;
};
