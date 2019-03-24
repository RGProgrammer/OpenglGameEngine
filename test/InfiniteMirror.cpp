#include "InfiniteMirror.h"

InfiniteMirror::  InfiniteMirror():Renderable(),m_ShaderProgram(0),m_VAO(0), m_VertexBuffer(0),
									m_NormalBuffer(0), m_TexCoordBuffer(0), m_IndexBuffer(0),
									m_FBO(),m_Texture(0),m_Camera(NULL)
{};
InfiniteMirror::~InfiniteMirror()
{
	this->Destroy();
	Renderable::~Renderable();
};
void InfiniteMirror::Destroy()
{
	if (m_GLRenderer) {
		if (m_ShaderProgram) {
			m_GLRenderer->DeleteGLProgram(m_ShaderProgram);
			m_ShaderProgram = 0;
		} 
		if (m_VAO) {
			m_GLRenderer->DeleteVertexArrays(1, &m_VAO);
			m_VAO = 0;
		}
		if (m_VertexBuffer) {
			m_GLRenderer->DeleteBuffers(1, &m_VertexBuffer);
			m_VertexBuffer = 0;
		}
		if (m_NormalBuffer) {
			m_GLRenderer->DeleteBuffers(1, &m_NormalBuffer);
			m_NormalBuffer = 0;
		}
		if (m_TexCoordBuffer) {
			m_GLRenderer->DeleteBuffers(1, &m_TexCoordBuffer);
			m_TexCoordBuffer = 0;
		}
		if (m_IndexBuffer) {
			m_GLRenderer->DeleteBuffers(1, &m_IndexBuffer);
			m_IndexBuffer = 0;
		}
		if (m_FBO) {
			m_GLRenderer->DeleteFrameBuffers(1, &m_FBO);
			m_FBO = 0;
		}
		if (m_Texture) {
			m_GLRenderer->DeleteTextures(1, &m_Texture);
			m_Texture=0;
		}
		m_GLRenderer = NULL;
	}
	if (m_Camera) {
		delete m_Camera;
		m_Camera = NULL;
	}
};
_bool InfiniteMirror::Init(GLRenderer* renderer)
{
	if (!renderer || !renderer->isInitialized())
		return false;
	m_GLRenderer = renderer;
	_float VertexBuffer[12] = {
		-2.0f,2.0f,0.0f,
		2.0f,2.0f,0.0f,
		2.0f,-2.0f,0.0f,
		-2.0f,-2.0f,0.0f};
	_float NormalBuffer[12] = { 
		0.0f,0.0f,1.0f,
		0.0f,0.0f,-1.0f, 
		0.0f,0.0f,-1.0f, 
		0.0f,0.0f,1.0f
	};
	_float TexCoordBuffer[8] = {
		0.0f,0.0f,
		1.0f,0.0f,
		1.0f,1.0f,
		0.0f,1.0f
	};
	_u32b IndexBuffer[6] = {
		1,2,0,
		1,3,2
	};
	
	//Loading shader program
	m_ShaderProgram = m_GLRenderer->CreateGLProgramFromFile("..//test//mirror.vs", "..//test//mirror.fs");
	if (!m_ShaderProgram) {
		this->Destroy();
		return false;
	}
	//Creating Buffers and VAO
	//VBO
	m_GLRenderer->GenBuffers(1, &m_VertexBuffer);
	if (!m_VertexBuffer) {
		this->Destroy();
		return false;
	}
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	m_GLRenderer->setBufferData(GL_ARRAY_BUFFER, 12 * sizeof(_float), VertexBuffer, GL_STATIC_DRAW);

	m_GLRenderer->GenBuffers(1, &m_NormalBuffer);
	if (!m_NormalBuffer) {
		this->Destroy();
		return false;
	}
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_NormalBuffer);
	m_GLRenderer->setBufferData(GL_ARRAY_BUFFER, 12 * sizeof(_float), NormalBuffer, GL_STATIC_DRAW);

	m_GLRenderer->GenBuffers(1, &m_TexCoordBuffer);
	if (!m_TexCoordBuffer) {
		this->Destroy();
		return false;
	}
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_TexCoordBuffer);
	m_GLRenderer->setBufferData(GL_ARRAY_BUFFER, 8 * sizeof(_float), TexCoordBuffer, GL_STATIC_DRAW);

	m_GLRenderer->GenBuffers(1, &m_IndexBuffer);
	if (!m_IndexBuffer) {
		this->Destroy();
		return false;
	}
	m_GLRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	m_GLRenderer->setBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(_u32b), IndexBuffer, GL_STATIC_DRAW);
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, 0);
	m_GLRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//VAO
	m_GLRenderer->GenVertexArrays(1, &m_VAO);
	if (!m_VAO) {
		this->Destroy();
		return false;
	}
	m_GLRenderer->BindVertexArray(m_VAO);

	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	m_GLRenderer->SetVertexAttribPointerF(0, 3, 0, 0);
	m_GLRenderer->EnableVertexAttribArray(0);

	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER,m_NormalBuffer);
	m_GLRenderer->SetVertexAttribPointerF(1, 3, 0, 0);
	m_GLRenderer->EnableVertexAttribArray(1);

	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER,m_TexCoordBuffer );
	m_GLRenderer->SetVertexAttribPointerF(2, 2, 0, 0);
	m_GLRenderer->EnableVertexAttribArray(2);

	m_GLRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	
	m_GLRenderer->BindVertexArray(0);
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, 0);
	m_GLRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//Creating FBO and its attachment 
	m_GLRenderer->GenFrameBuffers(1, &m_FBO);
	if (!m_FBO) {
		this->Destroy();
		return false;
	}
	m_GLRenderer->GenTextures2D(1, &m_Texture);
	if (!m_Texture) {
		this->Destroy();
		return false;
	}
	Image data = { 800,600,NULL };
	m_GLRenderer->BindTexture(m_Texture);
	m_GLRenderer->SetImageData2D(&data);
	m_GLRenderer->BindTexture(0);
	m_GLRenderer->BindFrameBuffer( m_FBO);
	m_GLRenderer->AttachTexturetoFrameBuffer(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);

	
	m_Camera = new PerspCamera(M_PI_2, 1.0f);
	m_Camera->setPosition(this->getPosition());
	m_Camera->setOrientation(this->getDirection(), this->getUp());
	
	m_DoesCastShadow = true;
	return true;
};
void InfiniteMirror::Render(Camera* camera)
{
	if (m_GLRenderer && m_GLRenderer->isInitialized()) {
		_s32b	Location = -1;
		//render scene from local camera perspective
		//m_GLRenderer->RenderScene(m_FBO, m_Camera);
		//bind shader program
		m_GLRenderer->SetShaderProgram(m_ShaderProgram);
		//uniforms
		Location = m_GLRenderer->GetUniformLocation(m_ShaderProgram, "World");
		m_GLRenderer->SetUniformvMtx(Location, this->getTransMtx());
		Location = m_GLRenderer->GetUniformLocation(m_ShaderProgram, "View");
		m_GLRenderer->SetUniformvMtx(Location, m_Camera->getViewMtx());
		Location = m_GLRenderer->GetUniformLocation(m_ShaderProgram, "Proj");
		m_GLRenderer->SetUniformvMtx(m_ShaderProgram, m_Camera->getProjectionMtx());
		Location = m_GLRenderer->GetUniformLocation(m_ShaderProgram, "Capture");
		m_GLRenderer->SetActiveTexture(0);
		//m_GLRenderer->BindTexture(m_Texture);
		//m_GLRenderer->BindTexture(m_GLRenderer->getLastFrameTexture());
		//m_GLRenderer->SetUniformSample(Location, 0);

		//render this object
		m_GLRenderer->BindVertexArray(m_VAO);
		m_GLRenderer->DrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,(void*) 0);

		//after render process
		
		m_GLRenderer->BindVertexArray(0);
		m_GLRenderer->SetShaderProgram(0);

	}
};
void	InfiniteMirror::CastShadow()
{

	if (!m_DoesCastShadow || !m_GLRenderer) {
		return;
	}
		//binding need VAO
	m_GLRenderer->BindVertexArray(m_VAO);
		//Draw
	m_GLRenderer->DrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
	m_GLRenderer->BindVertexArray(0);
};