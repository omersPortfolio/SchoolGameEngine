#pragma once
#include "Math/Vector.h"

namespace fw {

class Texture
{
public:
    Texture(const char* filename);
    virtual ~Texture();

    GLuint GetHandle() { return m_TextureHandle; }
    vec2 GetSize() { return vec2((float)width, (float)height); }  // Returns the size of texture

protected:
    GLuint m_TextureHandle = 0;
    int width;
    int height;
};

} // namespace fw
