#ifndef _RGP_REFLEX_OBJECT_
#define _RGP_REFLEX_OBJECT_

#include "..//Core//AdvancedActors//EnvMapProbe.h"
#include "..//Core//BaseActors//Renderable.h"
using namespace RGP_CORE;
class ReflexObject : public Renderable 
{
public:
	ReflexObject();
	ReflexObject(Vertex3d Pos);
	void Destroy();
	void Render(Camera* Selected);
	void CastShadow();
	_bool Init(GLRenderer* renderer,EnvMapProbe* Probe);
private:
	EnvMapProbe*		m_Probe;
	GLuint				m_ShaderProgram;
	GLuint				m_VAO;
	GLuint				m_NormalBuffer;
	GLuint				m_VertexBuffer;
	GLuint				m_IndexBuffer;


};
#endif 