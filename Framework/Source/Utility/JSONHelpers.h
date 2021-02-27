#pragma once

#include "Math/Vector.h"

namespace fw {

typedef rapidjson::PrettyWriter<rapidjson::StringBuffer> WriterType;

extern void JSONSaveVec2(WriterType& writer, const char* key, vec2 value);
extern void JSONSaveVec3(WriterType& writer, const char* key, vec3 value);

} // namespace fw
