#include "Grid.h"

RGP_LEVELBUILDER::Grid * RGP_LEVELBUILDER::Grid::CreateGrid(GLRenderer* renderer)
{
	Grid* singleton = NULL;
	//if (!singleton) {
		if (!renderer || !renderer->isInitialized())
			return NULL;
		singleton = new Grid();
		if (!singleton)
			return NULL;
		singleton->setRenderer(renderer);
		if (!singleton->Init()) {
			delete singleton;
			singleton = NULL;
			return NULL;
		}
	//}
	return singleton;
}
	

RGP_LEVELBUILDER::Grid::Grid():m_ShaderProgram(0),m_VAO(0),m_VertexBuffer(0)
{
}

_bool RGP_LEVELBUILDER::Grid::Init()
{
	_float vertices[88];
	m_ShaderProgram = m_GLRenderer->CreateGLProgramFromFile("..//Shaders//Grid.vs", "..//Shaders//Grid.fs");
	if (!m_ShaderProgram) 
		return false;
	
	for (_u32b i = 0; i <= 10 ; ++i) {
		vertices[i * 4    ] = -50.0f ;
		vertices[i * 4 + 1] = 50.0f - i * 10;

		vertices[i * 4 + 2] = 50.0f ;
		vertices[i * 4 + 3] = 50.0f -i * 10;

	}
	for (_u32b i = 11; i <= 21; ++i) {
		vertices[i * 4] = 50.0f - (i-11) * 10;
		vertices[i * 4 + 1] = 50.0f;

		vertices[i * 4 + 2] = 50.0f - (i-11) * 10;
		vertices[i * 4 + 3] = - 50.0f;
	}

	m_GLRenderer->GenBuffers(1, &m_VertexBuffer);
	if (!m_VertexBuffer)
		return false;
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	m_GLRenderer->setBufferData(GL_ARRAY_BUFFER, 88 * sizeof(_float), vertices, GL_STATIC_DRAW);


	m_GLRenderer->GenVertexArrays(1, &m_VAO);
	if (!m_VAO)
		return false;
	m_GLRenderer->BindVertexArray(m_VAO);
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	m_GLRenderer->SetVertexAttribPointerF(0, 2);
	m_GLRenderer->EnableVertexAttribArray(0);

	m_GLRenderer->BindVertexArray(0);

	return true;
}

RGP_LEVELBUILDER::Grid::~Grid()
{
	this->Destroy();
	//singleton = NULL;
}

void RGP_LEVELBUILDER::Grid::Destroy()
{
	if (m_ShaderProgram) {
		m_GLRenderer->DeleteGLProgram(m_ShaderProgram);
		m_ShaderProgram = 0;
	}
	if (m_VAO) {
		m_GLRenderer->DeleteVertexArrays(1,&m_VAO);
		m_VAO = 0;
	}
	if (m_VertexBuffer) {
		m_GLRenderer->DeleteBuffers(1,&m_VertexBuffer);
		m_VertexBuffer = 0;
	}
}
void RGP_LEVELBUILDER::Grid::Render(RGP_CORE::Camera * Selected)
{
	
	if (m_GLRenderer->getCurrentShaderProgram() != m_ShaderProgram) {
		m_GLRenderer->SetShaderProgram(m_ShaderProgram);
		m_GLRenderer->BindBufferBase(GL_UNIFORM_BUFFER, 0, m_GLRenderer->getCameratransformsUBO());
	}
	m_GLRenderer->BindVertexArray(m_VAO);
	m_GLRenderer->DrawArrays(GL_LINES, 0, 120);
}

void RGP_LEVELBUILDER::Grid::CastShadow()
{
}
