#pragma once

class PlayerController;

class Player : public fw::GameObject
{
public:
    Player(Player& original);
    Player(fw::Scene* pScene, PlayerController* pPlayerController, std::string name, vec3 pos, vec3 rot, vec3 scale, fw::Material* pMaterial);
    virtual ~Player();

    virtual void Update(float deltaTime) override;
    virtual void OnEvent(fw::Event* pEvent) override;

    virtual void ImGuiInspector() override;
    static const char* GetStaticClassType() { return "Player"; }

protected:
    PlayerController* m_pPlayerController = nullptr;

    // Debug.
    vec2 RightMouseClickTest = 0;
    vec2 LeftMouseClickTest = 0;

    // Temp way to parametrize player movement for the sake of demonstrating the object inspector.
    float m_Speed = 5;
    bool m_AllowedToJump = false;
};
