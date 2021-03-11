#include "FrameworkPCH.h"
#include "MeshComponent.h"

void fw::MeshComponent::Save(WriterType& writer)
{
    writer.Key("Name");
    writer.String("MeshComponent");
}

void fw::MeshComponent::Load(rapidjson::Value& component)
{
}
