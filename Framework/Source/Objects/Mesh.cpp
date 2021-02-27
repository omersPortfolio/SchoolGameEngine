#include "FrameworkPCH.h"

#include "Mesh.h"
#include "Objects/Camera.h"
#include "Objects/Texture.h"
#include "Utility/ShaderProgram.h"
#include "Utility/Helpers.h"
#include "Math/MyMatrix.h"

namespace fw {

Mesh::Mesh()
{
}

Mesh::Mesh(int primitiveType, int numVertices, const VertexFormat* pVertices)
{
    CreateShape(primitiveType, numVertices, pVertices);
}

Mesh::Mesh(vec3 aCubeSize)
{
    CreateCube(aCubeSize);
}

Mesh::Mesh(vec2 aWorldSize, ivec2 aVertCount)
{
    CreatePlane(aWorldSize, aVertCount);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &m_VBO);
}

void Mesh::Start(int primitiveType)
{
    if (!m_Editable)
    {
        m_Verts.clear();

        m_PrimitiveType = primitiveType;

        m_Editable = true;
        //LOG(INFO, "This Mesh is now editable");
    }
    else
    {
        //LOG(INFO, "This Mesh is already editable");
    }
}

void Mesh::AddVertex(const VertexFormat nVert)
{
    if (m_Editable)
    {
        m_Verts.push_back(nVert);

        //LOG(INFO, "Vertex Added");
    }
    else
    {
        //LOG(INFO, "This Mesh isn't editable");
    }
}

void Mesh::AddVertex(const vec3 pos, const vec2 uv)
{
    if (m_Editable)
    {
        m_Verts.push_back(VertexFormat(pos.x, pos.y, pos.z, uv.x, uv.y));

        //LOG(INFO, "Vertex Added");
    }
    else
    {
        //LOG(INFO, "This Mesh isn't editable");
    }
}

void Mesh::AddVertex(const vec3 pos, const float u, const float v)
{
    if (m_Editable)
    {
        m_Verts.push_back(VertexFormat(pos.x, pos.y, pos.z, u, v));

        //LOG(INFO, "Vertex Added");
    }
    else
    {
        //LOG(INFO, "This Mesh isn't editable");
    }
}

void Mesh::AddVertex(const float x, const float y, const float z, const float u, const float v)
{
    if (m_Editable)
    {
        m_Verts.push_back(VertexFormat(x, y, z, u, v));

        //LOG(INFO, "Vertex Added");
    }
    else
    {
        //LOG(INFO, "This Mesh isn't editable");
    }
}

void Mesh::AddVertex(const float x, const float y, const float z, const vec2 uv)
{
    if (m_Editable)
    {
        m_Verts.push_back(VertexFormat(x, y, z, uv.x, uv.y));

        //LOG(INFO, "Vertex Added");
    }
    else
    {
        //LOG(INFO, "This Mesh isn't editable");
    }
}

void Mesh::AddSprite(vec3 pos)
{
    if (m_Editable)
    {
        if (m_PrimitiveType != GL_TRIANGLES)
        {
            m_PrimitiveType = GL_TRIANGLES;
        }

        float halfSize = 0.2f;
        m_Verts.push_back(VertexFormat(pos.x - halfSize, pos.y - halfSize, pos.z, 0, 0)); // bl
        m_Verts.push_back(VertexFormat(pos.x - halfSize, pos.y + halfSize, pos.z, 0, 1)); // tl
        m_Verts.push_back(VertexFormat(pos.x + halfSize, pos.y + halfSize, pos.z, 1, 1)); // tr

        m_Verts.push_back(VertexFormat(pos.x - halfSize, pos.y - halfSize, pos.z, 0, 0)); // bl
        m_Verts.push_back(VertexFormat(pos.x + halfSize, pos.y + halfSize, pos.z, 1, 1)); // tr
        m_Verts.push_back(VertexFormat(pos.x + halfSize, pos.y - halfSize, pos.z, 1, 0)); // br

        //LOG(INFO, "Sprite Mesh Added");
    }
    else
    {
        //LOG(INFO, "This Mesh isn't editable");
    }
}

void Mesh::End()
{
    if (m_Editable)
    {
        glDeleteBuffers(1, &m_VBO);

        glGenBuffers(1, &m_VBO); // m_VBO is a GLuint.

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        m_NumVertices = (int)m_Verts.size();

        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * m_NumVertices, &m_Verts[0], GL_STATIC_DRAW);
        //CreateShape(GL_TRIANGLES, (int)m_Verts.size(), &m_Verts[0]);

        m_Editable = false;
        //LOG(INFO, "This Mesh is now Complete");
    }
    else
    {
        //LOG(INFO, "This Mesh has already Complete");
    }
}

void Mesh::CreateShape(int aPrimitiveType, int aNumVertices, const VertexFormat* pVertices)
{
    // Delete the old buffer if we had one.
    glDeleteBuffers(1, &m_VBO);

    // Generate a buffer for our vertex attributes.
    glGenBuffers(1, &m_VBO); // m_VBO is a GLuint.

    // Set this VBO to be the currently active one.
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    m_PrimitiveType = aPrimitiveType;
    m_NumVertices = aNumVertices;

    // Copy our attribute data into the VBO.
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * m_NumVertices, pVertices, GL_STATIC_DRAW);
}

void Mesh::CreateShape(int aPrimitiveType, int aNumVertices, const VertexFormat* pVertices, int aNumIndices, const unsigned int* pIndices)
{
    // Delete the old buffer if we had one.
    glDeleteBuffers(1, &m_IBO);

    // Generate a buffer for our indices.
    glGenBuffers(1, &m_IBO);

    // Set this IBO to be currently active
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

    m_NumIndices = aNumIndices;

    // Copy our indices into the IBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_NumIndices, pIndices, GL_STATIC_DRAW);

    CreateShape(aPrimitiveType, aNumVertices, pVertices);
}

void Mesh::CreateCube(vec3 aSize)
{
    const VertexFormat cubeVerts[] =
    {
        // Face One
        VertexFormat(-0.5f * aSize.x, -0.5f * aSize.y, -0.5f * aSize.z,   0.0f, 0.0f),  // lbf
        VertexFormat(-0.5f * aSize.x,  0.5f * aSize.y, -0.5f * aSize.z,   0.0f, 0.33f), // ltf
        VertexFormat(0.5f * aSize.x, -0.5f * aSize.y, -0.5f * aSize.z,   0.5f, 0.0f),  // rbf
        VertexFormat(0.5f * aSize.x,  0.5f * aSize.y, -0.5f * aSize.z,   0.5f, 0.33f), // rtf

        // Face Two
        VertexFormat(0.5f * aSize.x, -0.5f * aSize.y, -0.5f * aSize.z,   0.0f, 0.33f),
        VertexFormat(0.5f * aSize.x,  0.5f * aSize.y, -0.5f * aSize.z,   0.0f, 0.66f),
        VertexFormat(0.5f * aSize.x, -0.5f * aSize.y,  0.5f * aSize.z,   0.5f, 0.33f),
        VertexFormat(0.5f * aSize.x,  0.5f * aSize.y,  0.5f * aSize.z,   0.5f, 0.66f),

        // Face Three
        VertexFormat(0.5f * aSize.x, 0.5f * aSize.y, -0.5f * aSize.z,   0.0f, 0.66f),
        VertexFormat(-0.5f * aSize.x, 0.5f * aSize.y, -0.5f * aSize.z,   0.0f, 1.0f),
        VertexFormat(0.5f * aSize.x, 0.5f * aSize.y,  0.5f * aSize.z,   0.5f, 0.66f),
        VertexFormat(-0.5f * aSize.x, 0.5f * aSize.y,  0.5f * aSize.z,   0.5f, 1.0f),

        // Face Four
        VertexFormat(-0.5f * aSize.x,  0.5f * aSize.y, -0.5f * aSize.z,   0.5f, 0.66f),
        VertexFormat(-0.5f * aSize.x, -0.5f * aSize.y, -0.5f * aSize.z,   0.5f, 1.0f),
        VertexFormat(-0.5f * aSize.x,  0.5f * aSize.y,  0.5f * aSize.z,   1.0f, 0.66f),
        VertexFormat(-0.5f * aSize.x, -0.5f * aSize.y,  0.5f * aSize.z,   1.0f, 1.0f),

        // Face Five
        VertexFormat(-0.5f * aSize.x, -0.5f * aSize.y, -0.5f * aSize.z,   0.5f, 0.33f), // lbf
        VertexFormat(0.5f * aSize.x, -0.5f * aSize.y, -0.5f * aSize.z,   0.5f, 0.66f), // rbf
        VertexFormat(-0.5f * aSize.x, -0.5f * aSize.y,  0.5f * aSize.z,   1.0f, 0.33f), // lbb 
        VertexFormat(0.5f * aSize.x, -0.5f * aSize.y,  0.5f * aSize.z,   1.0f, 0.66f), // rbb

        // Face Six 
        VertexFormat(0.5f * aSize.x, -0.5f * aSize.y,  0.5f * aSize.z,   0.5f, 0.0f),
        VertexFormat(0.5f * aSize.x,  0.5f * aSize.y,  0.5f * aSize.z,   0.5f, 0.33f),
        VertexFormat(-0.5f * aSize.x, -0.5f * aSize.y,  0.5f * aSize.z,   1.0f, 0.0f),
        VertexFormat(-0.5f * aSize.x,  0.5f * aSize.y,  0.5f * aSize.z,   1.0f, 0.33f),
    };

    const unsigned int cubeIndices[] =
    {
        // Face One
        0, 1, 2, 2, 1, 3,

        // Face Two
        4, 5, 6, 6, 5, 7,

        // Face Three
        8, 9, 10, 10, 9, 11,

        // Face Four
        12, 13, 14, 14, 13, 15,

        // Face Five
        16, 17, 18, 18, 17, 19,

        // Face Six
        20, 21, 22, 22, 21, 23,
    };

    const int meshPrimType = GL_TRIANGLES;
    const int meshNumVerts = sizeof(cubeVerts) / sizeof(fw::VertexFormat);
    const int meshNumIndices = sizeof(cubeIndices) / sizeof(unsigned int);

    CreateShape(meshPrimType, meshNumVerts, cubeVerts, meshNumIndices, cubeIndices);
}

void Mesh::CreatePlane(vec2 aWorldSize, ivec2 aVertCount)
{
    std::vector<VertexFormat> verts;
    std::vector<unsigned int> indices;

    verts.reserve(aVertCount.x * aVertCount.y);
    indices.reserve((aVertCount.x - 1) * (aVertCount.y - 1) * 6);

    float stepSizeX = aWorldSize.x / (aVertCount.x - 1);
    float stepSizeY = aWorldSize.y / (aVertCount.y - 1);
    float uvSizeX = 5.0f / (aVertCount.x - 1);
    float uvSizeY = 5.0f / (aVertCount.y - 1);

    for (int y = 0; y < aVertCount.y; y++)
    {
        for (int x = 0; x < aVertCount.x; x++)
        {
            verts.push_back(VertexFormat(stepSizeX * x, 0, stepSizeY * y, uvSizeX * x, uvSizeY * y));
        }
    }

    for (int y = 0; y < aVertCount.y - 1; y++)
    {
        for (int x = 0; x < aVertCount.x - 1; x++)
        {
            unsigned int index = y * aVertCount.x + x;

            indices.push_back(index);
            indices.push_back(index + aVertCount.x);
            indices.push_back(index + 1);
            indices.push_back(index + 1);
            indices.push_back(index + aVertCount.x);
            indices.push_back(index + aVertCount.x + 1);
        }
    }

    CreateShape(GL_TRIANGLES, (int)verts.size(), &verts[0], (int)indices.size(), &indices[0]);
}

void Mesh::CreateFromOBJ(const char* filename)
{
    long length = 0;
    char* buffer = LoadCompleteFile(filename, &length);
    if (buffer == 0 || length == 0)
    {
        delete[] buffer;
        return;
    }

    std::vector<vec3> positions;
    std::vector<vec2> uvs;
    std::vector<vec3> normals;
    std::vector<VertexFormat> verts;

    char* next_token = 0;
    char* line = strtok_s(buffer, "\n", &next_token);
    while (line)
    {
        if (line[0] == 'v')
        {
            if (line[1] == 't')
            {
                vec2 value;
                sscanf_s(line, "vt %f %f", &value.x, &value.y);
                uvs.push_back(value);
            }
            else if (line[1] == 'n')
            {
                vec3 value;
                sscanf_s(line, "vn %f %f %f", &value.x, &value.y, &value.z);
                normals.push_back(value);
            }
            else if (line[1] == ' ')
            {
                vec3 value;
                sscanf_s(line, "v %f %f %f", &value.x, &value.y, &value.z);
                positions.push_back(value);
            }
        }

        // "f 1/1/1 2/2/1 3/3/1"
        if (line[0] == 'f')
        {
            ivec3 v1, v2, v3;
            sscanf_s(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1.x, &v1.y, &v1.z, &v2.x, &v2.y, &v2.z, &v3.x, &v3.y, &v3.z);

            verts.push_back(VertexFormat(positions[v1.x - 1], uvs[v1.y - 1]));
            verts.push_back(VertexFormat(positions[v2.x - 1], uvs[v2.y - 1]));
            verts.push_back(VertexFormat(positions[v3.x - 1], uvs[v3.y - 1]));
        }

        line = strtok_s(0, "\n", &next_token);
    }

    CreateShape(GL_TRIANGLES, (int)verts.size(), &verts[0]);

    delete[] buffer;
}

void Mesh::SetUniform1f(ShaderProgram* pShader, char* name, float value)
{
    int loc = glGetUniformLocation(pShader->GetProgram(), name);
    glUniform1f(loc, value);
}

void Mesh::SetUniform2f(ShaderProgram* pShader, char* name, vec2 value)
{
    int loc = glGetUniformLocation(pShader->GetProgram(), name);
    glUniform2f(loc, value.x, value.y);
}

void Mesh::SetUniform3f(ShaderProgram* pShader, char* name, vec3 value)
{
    int loc = glGetUniformLocation(pShader->GetProgram(), name);
    glUniform3f(loc, value.x, value.y, value.z);
}

void Mesh::SetUniform4f(ShaderProgram* pShader, char* name, vec4 value)
{
    int loc = glGetUniformLocation(pShader->GetProgram(), name);
    glUniform4f(loc, value.x, value.y, value.z, value.w);
}

void Mesh::SetUniform1i(ShaderProgram* pShader, char* name, int value)
{
    int loc = glGetUniformLocation(pShader->GetProgram(), name);
    glUniform1i(loc, value);
}

void Mesh::Draw(Camera* pCamera, vec3 pos, vec3 rot, vec3 scale, Material* pMaterial)
{
    Draw(pCamera, pos, rot, scale, pMaterial->GetShader(), pMaterial->GetTexture(), pMaterial->GetColor(), pMaterial->GetUVScale(), pMaterial->GetUVOffset());
}

void Mesh::Draw(Camera* pCamera, vec3 pos, vec3 rot, vec3 scale, ShaderProgram* pShader, Texture* pTexture, Color color, vec2 UVScale, vec2 UVOffset)
{
    glUseProgram(pShader->GetProgram());

    // Set this VBO to be the currently active one.
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // Set this IBO to be the currently active one.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

    // Get the attribute variable’s location from the shader.
    GLint loc = glGetAttribLocation(pShader->GetProgram(), "a_Position");
    if (loc != -1)
    {
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 20, (void*)0);
    }

    loc = glGetAttribLocation(pShader->GetProgram(), "a_UVCoord");
    if (loc != -1)
    {
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 20, (void*)12);
    }

    // Setup our uniforms.
    {
        SetUniform1f(pShader, "u_Time", (float)GetSystemTimeSinceGameStart());
        SetUniform4f(pShader, "u_Color", vec4(color.r, color.g, color.b, color.a));

        SetUniform2f(pShader, "u_UVScale", UVScale);
        SetUniform2f(pShader, "u_UVOffset", UVOffset);

        SetUniform3f(pShader, "u_CameraPosition", pCamera->GetPosition());
        SetUniform2f(pShader, "u_ProjectionScale", pCamera->GetProjectionScale());

        // Transforms.
        int loc = glGetUniformLocation(pShader->GetProgram(), "u_WorldTransform");
        MyMatrix mat;
        mat.CreateSRT(scale, rot, pos);
        glUniformMatrix4fv(loc, 1, false, &mat.m11);

        int locView = glGetUniformLocation(pShader->GetProgram(), "u_ViewTransform");
        const MyMatrix& viewMat = pCamera->GetViewTransform();
        glUniformMatrix4fv(locView, 1, false, &viewMat.m11);

        int locProj = glGetUniformLocation(pShader->GetProgram(), "u_ProjectionMatrix");
        MyMatrix projMat;
        projMat.CreateOrtho(-pCamera->GetProjectionScale().x, pCamera->GetProjectionScale().x,
                            -pCamera->GetProjectionScale().y, pCamera->GetProjectionScale().y,
                            -1000, 1000);
        //projMat.CreatePerspectiveVFoV(45.0f, 1280 / 720.0f, 0.01f, 1000.0f);
        glUniformMatrix4fv(locProj, 1, false, &projMat.m11);

        if (pTexture != nullptr)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, pTexture->GetHandle());
            SetUniform1i(pShader, "u_Texture", 0);
        }
    }

    // Draw the primitive.
    if (m_NumIndices == 0)
    {
        glDrawArrays(m_PrimitiveType, 0, m_NumVertices);
    }
    else
    {
        glDrawElements(m_PrimitiveType, m_NumIndices, GL_UNSIGNED_INT, (void*)0);
    }
}

} // namespace fw
