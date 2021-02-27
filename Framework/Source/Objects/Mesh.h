#pragma once

#include "Math/Vector.h"
#include "Utility/Color.h"

namespace fw {

class Camera;
class ShaderProgram;
class Texture;
class Material;

struct VertexFormat
{
    float x;
    float y;
    float z;
    float u;
    float v;

    VertexFormat(float nx, float ny, float nz, float nu, float nv)
    {
        x = nx;
        y = ny;
        z = nz;
        u = nu;
        v = nv;
    }

    VertexFormat(vec3 pos, vec2 uv)
    {
        x = pos.x;
        y = pos.y;
        z = pos.z;
        u = uv.x;
        v = uv.y;
    }
};

class Mesh
{
public:
    Mesh();
    // Creates a mesh from a vertex array.
    Mesh(int primitiveType, int numVertices, const VertexFormat* pVertices);
    // Creates a cube.
    Mesh(vec3 cubeSize);
    // Creates a plane.
    Mesh(vec2 worldSize, ivec2 vertCount);
    virtual ~Mesh();

    void Start(int primitiveType);
    void AddVertex(const VertexFormat nVert);
    void AddVertex(const vec3 pos, const vec2 uv);
    void AddVertex(const vec3 pos, const float u, const float v);
    void AddVertex(const float x, const float y, const float z, const float u, const float v);
    void AddVertex(const float x, const float y, const float z, const vec2 uv);
    void AddSprite(vec3 pos);
    void End();

    void CreateShape(int primitiveType, int numVertices, const VertexFormat* pVertices);
    void CreateShape(int primitiveType, int numVertices, const VertexFormat* pVertices, int numIndices, const unsigned int* pIndices);
    void CreateCube(vec3 size);
    void CreatePlane(vec2 worldSize, ivec2 vertCount);
    void CreateFromOBJ(const char* filename);

    void SetUniform1f(ShaderProgram* pShader, char* name, float value);
    void SetUniform2f(ShaderProgram* pShader, char* name, vec2 value);
    void SetUniform3f(ShaderProgram* pShader, char* name, vec3 value);
    void SetUniform4f(ShaderProgram* pShader, char* name, vec4 value);
    void SetUniform1i(ShaderProgram* pShader, char* name, int value);

    void Draw(Camera* pCamera, vec3 pos, vec3 rot, vec3 scale, Material* pMaterial);
    void Draw(Camera* pCamera, vec3 pos, vec3 rot, vec3 scale, ShaderProgram* pShader, Texture* pTexture, Color color, vec2 UVScale, vec2 UVOffset);

protected:
    GLuint m_IBO = 0;
    GLuint m_VBO = 0;

    std::vector<VertexFormat> m_Verts;

    int m_NumVertices = 0;
    int m_NumIndices = 0;
    int m_PrimitiveType = GL_POINTS;

    bool m_Editable = false;
};

} // namespace fw
