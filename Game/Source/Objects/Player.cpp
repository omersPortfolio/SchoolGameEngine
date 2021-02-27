#include "GamePCH.h"

#include "Objects/Player.h"
#include "Objects/PlayerController.h"
#include "Game.h"

Player::Player(fw::Scene* pScene, PlayerController* pPlayerController, std::string name, vec3 pos, vec3 rot, vec3 scale, fw::Material* pMaterial)
    : fw::GameObject(pScene, name, pos, rot, scale, pMaterial)
    , m_pPlayerController(pPlayerController)
{
    className = "Player";

    m_pScene->GetGameCore()->GetEventManager()->RegisterEventListener(this, fw::PhysicsCollisionEvent::GetStaticEventType());
}

Player::Player(Player& original)
    : fw::GameObject(original)
{
    className = "Player";

    m_pScene->GetGameCore()->GetEventManager()->RegisterEventListener(this, fw::PhysicsCollisionEvent::GetStaticEventType());
    m_pPlayerController = original.m_pPlayerController;
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
    GameObject::Update(deltaTime);

    if (m_pPhysicsBody == nullptr)
    {
        float speed = 2.0f;

        vec2 dir = vec2(0.f, 0.f);

        if (m_pPlayerController->IsHeld(PlayerController::Mask::Up))
        {
            dir.y += 1;
        }
        if (m_pPlayerController->IsHeld(PlayerController::Mask::Down))
        {
            dir.y += -1;
        }
        if (m_pPlayerController->IsHeld(PlayerController::Mask::Left))
        {
            dir.x += -1;
        }
        if (m_pPlayerController->IsHeld(PlayerController::Mask::Right))
        {
            dir.x += 1;
        }

        m_pMaterial->SetUVScale(vec2(64.0 / 1024.0, 64.0 / 512.0));
        m_pMaterial->SetUVOffset(vec2(780.0 / 1024.0, 383.0 / 512.0));

        dir.Normalize();

        m_Position += dir * speed * deltaTime;
    }
    else
    {
        if (m_pPlayerController->IsHeld(PlayerController::Mask::Right))
        {
            m_pPhysicsBody->ApplyForceToCenter(vec3(1, 0, 0) * m_Speed);
        }

        if (m_pPlayerController->IsHeld(PlayerController::Mask::Left))
        {
            m_pPhysicsBody->ApplyForceToCenter(vec3(-1, 0, 0) * m_Speed);
        }

        if (m_pPlayerController->WasNewlyPressed(PlayerController::Mask::Up))
        {
            if (m_AllowedToJump)
            {
                m_pPhysicsBody->ApplyImpulseToCenter(vec3(0, 1, 0) * m_Speed);
                m_AllowedToJump = false;
            }
        }
    }
}

void Player::OnEvent(fw::Event* pEvent)
{
    if (pEvent->GetType() == fw::PhysicsCollisionEvent::GetStaticEventType())
    {
        m_AllowedToJump = true;
    }
}

void Player::ImGuiInspector()
{
    GameObject::ImGuiInspector();

    ImGui::SliderFloat("Speed", &m_Speed, 0.f, 10.f, "%.2f");
}
