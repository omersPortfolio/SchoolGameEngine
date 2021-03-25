#include "FrameworkPCH.h"

#include "Texture.h"
//#define STB_IMAGE_IMPLEMENTATION // Defined in ImFileDialog.cpp
#include "../../Libraries/stb/stb_image.h"

namespace fw {

Texture::Texture(const char* filename)
{
    // Load texture from disk.
    int numComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filename, &m_Width, &m_Height, &numComponents, 4);

    if (data != nullptr)
    {
        m_FreeTextureWhenDestroyed = true;

        // Create OpenGL Texture.
        glGenTextures(1, &m_TextureHandle);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_TextureHandle);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);
    }
}

Texture::Texture(GLuint textureHandle)
{
    m_TextureHandle = textureHandle;
    m_FreeTextureWhenDestroyed = false;
}

Texture::~Texture()
{
    if( m_FreeTextureWhenDestroyed )
    {
        glDeleteTextures(1, &m_TextureHandle);
    }
}

} // namespace fw
