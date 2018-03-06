#ifndef _INFINITE_MIRROR_H_
#define _INFINITE_MIRROR_H_

#include "..//Core//Graphics//GLRenderer.h"

using namespace RGP_CORE;
//trying to implement reflexion and this class is for testing


class InfiniteMirror : public Renderable
{
public:
	InfiniteMirror();
	~InfiniteMirror();
	void Destroy();
	_bool Init(GLRenderer* renderer);
	void Render(Camera* camera);
	void CastShadow() ;
	
private :
	_u32b			m_ShaderProgram;
	GLuint			m_VAO;
	GLuint			m_VertexBuffer;
	GLuint			m_NormalBuffer;
	GLuint			m_TexCoordBuffer;
	GLuint			m_IndexBuffer;
	GLuint			m_FBO;
	GLuint			m_Texture;
	PerspCamera*	m_Camera;
};

#endif
