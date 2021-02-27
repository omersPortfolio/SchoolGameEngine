#pragma once

class fw::PhysicsWorld;
class fw::PhysicsBody;
class Game;

class GameScene : public fw::Scene
{
public:
    GameScene(fw::GameCore* pGameCore);
    virtual ~GameScene();

    virtual void Init() override;
    virtual void StartFrame(float deltaTime) override;
    virtual void OnEvent(fw::Event* pEvent) override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

    void Load(const char* filename);

    Game* GetGame();

    void SetName(std::string NewName);

protected:
    fw::ParticleEmitter* m_Particles = nullptr;

protected:
    virtual void DrawNewObjectButtons() override;
};
