#include "FrameworkPCH.h"
#include "JSONHelpers.h"
#include "Math/Vector.h"

namespace fw {

void JSONSaveCharArray(WriterType& writer, const char* key, const char* type)
{
    writer.Key(key);
    writer.String(type);
}

void JSONSaveInt(WriterType& writer, const char* key, int value)
{
    writer.Key(key);
    writer.Int(value);
}

void JSONSaveVec2(WriterType& writer, const char* key, vec2 value)
{
    writer.Key(key);
    writer.StartArray();
    writer.Double(value.x);
    writer.Double(value.y);
    writer.EndArray();
}

void JSONSaveVec3(WriterType& writer, const char* key, vec3 value)
{
    writer.Key(key);
    writer.StartArray();
    writer.Double(value.x);
    writer.Double(value.y);
    writer.Double(value.z);
    writer.EndArray();
}

void JSONLoadInt(rapidjson::Value& object, const char* key, int* value)
{
    assert( value != nullptr );

    if( object.HasMember( key ) )
    {
        *value = object[key].GetInt();
    }
}

void JSONLoadVec2(rapidjson::Value& object, const char* key, vec2* value)
{
    assert( value != nullptr );

    if( object.HasMember( key ) )
    {
        value->x = object[key][0].GetFloat();
        value->y = object[key][1].GetFloat();
    }
}

void JSONLoadVec3(rapidjson::Value& object, const char* key, vec3* value)
{
    assert( value != nullptr );

    if( object.HasMember( key ) )
    {
        value->x = object[key][0].GetFloat();
        value->y = object[key][1].GetFloat();
        value->z = object[key][2].GetFloat();
    }
}

} // namespace fw
