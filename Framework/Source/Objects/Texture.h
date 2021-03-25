#pragma once
#include "Math/Vector.h"

namespace fw {

class Texture
{
public:
    Texture(const char* filename);
    Texture(GLuint textureHandle);
    virtual ~Texture();

    GLuint GetHandle() { return m_TextureHandle; }
    vec2 GetSize() { return vec2((float)m_Width, (float)m_Height); }  // Returns the size of texture

protected:
    GLuint m_TextureHandle = 0;
    int m_Width = 0;
    int m_Height = 0;

    bool m_FreeTextureWhenDestroyed = false;
};

} // namespace fw
