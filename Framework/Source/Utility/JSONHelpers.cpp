#include "FrameworkPCH.h"
#include "JSONHelpers.h"
#include "Math/Vector.h"

namespace fw {

extern void JSONSaveVec2(WriterType& writer, const char* key, vec2 value)
{
    writer.Key(key);
    writer.StartArray();
    writer.Double(value.x);
    writer.Double(value.y);
    writer.EndArray();
}

extern void JSONSaveVec3(WriterType& writer, const char* key, vec3 value)
{
    writer.Key(key);
    writer.StartArray();
    writer.Double(value.x);
    writer.Double(value.y);
    writer.Double(value.z);
    writer.EndArray();
}

} // namespace fw
