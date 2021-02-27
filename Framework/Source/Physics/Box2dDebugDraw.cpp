#include "FrameworkPCH.h"
#include "Box2dDebugDraw.h"
#include "Objects/Material.h"
#include "Utility/ShaderProgram.h"

namespace fw
{

Box2dDebugDraw::Box2dDebugDraw(Material* pMaterial)
{
    m_pMaterial = pMaterial;
}

Box2dDebugDraw::~Box2dDebugDraw()
{}

void Box2dDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    glUseProgram(m_pShader->GetProgram());

    /*glDisable(GL_CULL_FACE & GL_DEPTH_TEST);
    glDrawArrays();
    glEnable(GL_CULL_FACE & GL_DEPTH_TEST);*/
}

void Box2dDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{}

void Box2dDebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{}

void Box2dDebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{}

void Box2dDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{}

//void Box2dDebugDraw::SetViewAndProj(vec4* view, vec4* proj)
//{
//}

void Box2dDebugDraw::DrawTransform(const b2Transform& xf)
{}

void Box2dDebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{}

} // namespace fw
