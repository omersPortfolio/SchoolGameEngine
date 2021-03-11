#pragma once

#include "Math/Vector.h"
#include "Utility/Color.h"

namespace fw {

class ShaderProgram;
class Texture;


class Material
{
public:
    Material(ShaderProgram* pShaderProgram, Texture* pTexture, Color color, vec2 uvScale, vec2 uvOffset);
    ~Material();

    ShaderProgram* GetShader() { return m_pShaderProgram; }
    Texture* GetTexture() { return m_pTexture; }
    Color GetColor() { return m_Color; }
    vec2 GetUVScale() { return m_UVScale; }
    vec2 GetUVOffset() { return m_UVOffset; }

    void SetTexture(Texture* pTexture) { m_pTexture = pTexture; }
    void SetColor(Color color) { m_Color = color; }
    void SetShader(ShaderProgram* pShader) { m_pShaderProgram = pShader; } 
    void SetUVScale(vec2 uvScale) { m_UVScale = uvScale; }
    void SetUVOffset(vec2 uvOffset) { m_UVOffset = uvOffset; }

protected:

    ShaderProgram* m_pShaderProgram = nullptr;
    Texture* m_pTexture = nullptr;
    Color m_Color = Color(1, 1, 1, 1);
    vec2 m_UVScale = vec2(1, 1);
    vec2 m_UVOffset = vec2(0, 0);
};

} // namespace fw
