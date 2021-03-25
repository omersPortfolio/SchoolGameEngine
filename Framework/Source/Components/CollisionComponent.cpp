#include "FrameworkPCH.h"

#include "CollisionComponent.h"
#include "Components/TransformComponent.h"
#include "Objects/GameObject.h"
#include "Scene/Scene.h"
#include "Math/Vector.h"

namespace fw {

CollisionComponent::CollisionComponent()
{
}

CollisionComponent::CollisionComponent(CollisionComponent* component)
{
    m_pGameObject = component->GetGameObject();
    m_Offset = component->GetOffset();
    m_ShapeType = component->GetShapeType();
    m_BodyType = component->GetBodyType();
    m_Dimensions = component->GetDimensions();
    m_Density = component->GetDensity();
    Init();
}

CollisionComponent::CollisionComponent(vec3 offset, ShapeType shapeType, BodyType bodyType, vec3 dimensions, float density)
{
    m_Offset = offset;
    m_ShapeType = shapeType;
    m_BodyType = bodyType;
    m_Dimensions = dimensions;
    m_Density = density;
}

CollisionComponent::~CollisionComponent()
{
    delete m_pPhysicsBody;
}

void CollisionComponent::Init()
{
    if (m_pGameObject != nullptr)
    {
        CreatePhysicsBody();
        SetPositionPhysicsBody();
    }
}

void CollisionComponent::Update(float deltaTime)
{
    SetPositionGameObject();
}

void CollisionComponent::Save(WriterType& writer)
{
    Component::Save(writer);

    JSONSaveVec3(writer, "Offset", m_Offset);

    writer.Key("ShapeType");
    writer.Int(m_ShapeType);

    writer.Key("BodyType");
    writer.Int(m_BodyType);

    writer.Key("Density");
    writer.Double(m_Density);

    JSONSaveVec3(writer, "Dimensions", m_Dimensions);
}

void CollisionComponent::Load(rapidjson::Value& component)
{
    if (component.HasMember("Offset"))
    {
        m_Offset.x = component["Offset"].GetArray()[0].GetFloat();
        m_Offset.y = component["Offset"].GetArray()[1].GetFloat();
        m_Offset.z = component["Offset"].GetArray()[2].GetFloat();
    }

    if (component.HasMember("ShapeType"))
    {
        m_ShapeType = (ShapeType)component["ShapeType"].GetInt();
    }

    if (component.HasMember("BodyType"))
    {
        m_BodyType = (BodyType)component["BodyType"].GetInt();
    }

    if (component.HasMember("Density"))
    {
        m_Density = component["Density"].GetFloat();
    }

    if (component.HasMember("Dimensions"))
    {
        m_Dimensions.x = component["Dimensions"].GetArray()[0].GetFloat();
        m_Dimensions.y = component["Dimensions"].GetArray()[1].GetFloat();
        m_Dimensions.z = component["Dimensions"].GetArray()[2].GetFloat();
    }
}

void CollisionComponent::AddToInspector()
{
    ImGui::Separator();

    ImGui::Text("Collision Component");

    //if (ImGui::DragFloat2("Offset", &m_Offset.x, 0.05f))
    //{
    //    // TODO: move this specific fixture within the body.
    //    SetPositionPhysicsBody();
    //};

    ImGui::Text("Shape Type: %s", ShapeTypeStrings[m_ShapeType]);
    ImGui::Text("Body Type: %s", BodyTypeStrings[m_BodyType]);

    if (ImGui::DragFloat("Density", &m_Density, 0.05f))
    {
    };

    if (ImGui::DragFloat3("Dimensions", &m_Dimensions.x, 0.05f))
    {
    };

    ImGui::Separator();
}

void CollisionComponent::CreatePhysicsBody()
{
    if (m_pGameObject != nullptr)
    {
        vec3 position = m_pGameObject->GetTransform()->GetPosition() + m_Offset;
        vec3 scale = m_pGameObject->GetTransform()->GetScale();

        switch (m_ShapeType)
        {
        case ShapeType::Circle:
            m_pPhysicsBody = m_pGameObject->GetScene()->GetWorld()->CreateBody(position, m_BodyType, true);
            m_pPhysicsBody->CreateCircle(scale.x);
            m_pPhysicsBody->SetDensity(m_Density);
            break;

        case ShapeType::Box:
            m_pPhysicsBody = m_pGameObject->GetScene()->GetWorld()->CreateBody(position, m_BodyType, true);
            m_pPhysicsBody->CreateBox(scale.x, scale.y);
            m_pPhysicsBody->SetDensity(m_Density);
            break;
        }
    }
}

//vec3 CollisionComponent::GetPosition()
//{
//    if (m_pPhysicsBody)
//    {
//        return m_pPhysicsBody->GetPosition();
//    }
//
//    return vec3(0, 0, 0);
//}
//
//void CollisionComponent::SetPosition(vec3 position)
//{
//    if (m_pPhysicsBody)
//    {
//        m_pPhysicsBody->SetPosition(position);
//    }
//}

void CollisionComponent::SetPositionPhysicsBody()
{
    if (m_pPhysicsBody && m_pGameObject)
    {
        vec3 position = m_pGameObject->GetTransform()->GetPosition() + m_Offset;
        m_pPhysicsBody->SetPosition(position);
    }
}

void CollisionComponent::SetPositionGameObject()
{
    if (m_pPhysicsBody && m_pGameObject)
    {
        vec3 position = m_pPhysicsBody->GetPosition() - m_Offset;
        m_pGameObject->GetTransform()->SetPosition(position.XY());

        vec3 rotation = m_pPhysicsBody->GetRotation();
        m_pGameObject->GetTransform()->SetRotation( rotation );
    }
}

} // namespace fw
