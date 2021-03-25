#pragma once
#include <map>
#include "Mesh.h"   //included for VertexFormat. Maybe Bad?

namespace fw {

class Material;
class Mesh;
class ShaderProgram;
class Texture;

class ResourceManager
{
    friend class ResourcesPanel;

public:
    ResourceManager();
    ~ResourceManager();

    Mesh* GetMesh(std::string name) { return m_pMeshes[name]; }
    ShaderProgram* GetShader(std::string name) { return m_pShaders[name]; }
    Texture* GetTexture(std::string name) { return m_pTextures[name]; }
    Material* GetMaterial(std::string name) { return m_pMaterials[name]; }

    void AddMesh(const char* name, Mesh* pMesh);
    void AddMesh(const char* name, int primitiveType, int numVertices, const VertexFormat* pVertices);

    void AddShader(const char* name, ShaderProgram* pShader);
    void AddShader(const char* name, const char* vertFilename, const char* fragFilename);

    void AddTexture(const char* name, Texture* pTexture);
    void AddTexture(const char* name, const char* filename);

    void AddMaterial(const char* name, Material* pMaterial);
    void AddMaterial(const char* name, ShaderProgram* pShaderProgram, Color pColor);
    void AddMaterial(const char* name, ShaderProgram* pShaderProgram, Texture* pTexture, Color pColor, vec2 m_UVScale, vec2 pUVOffset);

    std::map<std::string, Texture*>* GetTexturesMap_TempRemoveMe() { return &m_pTextures; }

    std::string FindMeshName(Mesh* pMesh);
    std::string FindShaderName(ShaderProgram* pShader);
    std::string FindTextureName(Texture* pTexture);
    std::string FindMaterialName(Material* pMaterial);

protected:
    std::map<std::string, Mesh*> m_pMeshes;
    std::map<std::string, ShaderProgram*> m_pShaders;
    std::map<std::string, Texture*> m_pTextures;
    std::map<std::string, Material*> m_pMaterials;
};

} // namespace fw
