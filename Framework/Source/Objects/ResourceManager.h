#pragma once
#include <map>
#include "Mesh.h"   //included for VertexFormat. Maybe Bad?

namespace fw {

class ShaderProgram;
class Mesh;
class Texture;
class Material;

class ResourceManager
{
    friend class ResourcesPanel;

public:
    ResourceManager();
    ~ResourceManager();

    ShaderProgram* GetShader(std::string name) { return m_pShaders[name]; }
    Mesh* GetMesh(std::string name) { return m_pMeshes[name]; }
    Texture* GetTexture(std::string name) { return m_pTextures[name]; }
    Material* GetMaterial(std::string name) { return m_pMaterials[name]; }

    void AddShader(const char* name, ShaderProgram* pShader);
    void AddShader(const char* name, const char* vertFilename, const char* fragFilename);

    void AddMesh(const char* name, Mesh* pMesh);
    void AddMesh(const char* name, int primitiveType, int numVertices, const VertexFormat* pVertices);

    void AddTexture(const char* name, Texture* pTexture);
    void AddTexture(const char* name, const char* filename);

    void AddMaterial(const char* name, Material* pMaterial);
    void AddMaterial(const char* name, ShaderProgram* pShaderProgram, Texture* pTexture, Color pColor, vec2 m_UVScale, vec2 pUVOffset);

    std::map<std::string, Texture*>* GetTexturesMap_TempRemoveMe() { return &m_pTextures; }

    std::string FindTextureName(Texture* pTexture);

protected:
    std::map<std::string, ShaderProgram*> m_pShaders;
    std::map<std::string, Mesh*> m_pMeshes;
    std::map<std::string, Texture*> m_pTextures;
    std::map<std::string, Material*> m_pMaterials;
};

} // namespace fw
