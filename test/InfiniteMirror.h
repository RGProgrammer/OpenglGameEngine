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
	_u32b			m_VAO;
	_u32b			m_VertexBuffer;
	_u32b			m_NormalBuffer;
	_u32b			m_TexCoordBuffer;
	_u32b			m_IndexBuffer;
	_u32b			m_FBO;
	_u32b			m_Texture;
	PerspCamera*	m_Camera;
};

#endif
