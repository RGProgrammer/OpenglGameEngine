#include ".//ReflexObject.h"

ReflexObject::ReflexObject() :Renderable(), m_Probe(NULL),m_ShaderProgram(0),m_VAO(0),
										m_NormalBuffer(0),m_VertexBuffer(0)
{};
ReflexObject::ReflexObject(Vertex3d Pos) :Renderable(), m_Probe(NULL), m_ShaderProgram(0), m_VAO(0),
											m_NormalBuffer(0), m_VertexBuffer(0)
{};
void ReflexObject::Destroy()
{
	if (m_GLRenderer) {
		if (m_ShaderProgram) {
			m_GLRenderer->DeleteGLProgram(m_ShaderProgram);
			m_ShaderProgram = 0;
		}
		if (m_VAO) {
			m_GLRenderer->DeleteVertexArrays(1,&m_VAO);
			m_VAO = 0;
		}
		if (m_NormalBuffer){
			m_GLRenderer->DeleteBuffers(1, &m_NormalBuffer);
			m_NormalBuffer = 0;
		}
		if (m_VertexBuffer) {
			m_GLRenderer->DeleteBuffers(1, &m_VertexBuffer);
			m_VertexBuffer = 0;
		}
		Renderable::Destroy();
	}
};
void ReflexObject::Render(Camera* Selected)
{
	if (m_GLRenderer) {
		//m_Probe->GenerateEnvMap();
		//before render
		m_GLRenderer->SetShaderProgram(m_ShaderProgram);
		GLint Location = -1;
		m_GLRenderer->GetUniformLocation(m_ShaderProgram, "World");
		m_GLRenderer->SetUniformvMtx(Location, this->getTransMtx());
		m_GLRenderer->GetUniformLocation(m_ShaderProgram, "View");
		m_GLRenderer->SetUniformvMtx(Location, Selected->getViewMtx());
		m_GLRenderer->GetUniformLocation(m_ShaderProgram, "Proj");
		m_GLRenderer->SetUniformvMtx(Location, Selected->getProjectionMtx());
		m_GLRenderer->GetUniformLocation(m_ShaderProgram, "CameraPos");
		m_GLRenderer->SetUniform3F(Location, Selected->getPosition().x,
			Selected->getPosition().y, Selected->getPosition().z);
		m_GLRenderer->GetUniformLocation(m_ShaderProgram, "Probe");
		m_GLRenderer->SetActiveTexture(0);
		m_GLRenderer->BindTexture(m_Probe->getTexture(), false);
		m_GLRenderer->SetUniformSample(Location, 0);
		//render
		m_GLRenderer->BindVertexArray(m_VAO);
		m_GLRenderer->DrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		//after render
		m_GLRenderer->SetShaderProgram(0);
		m_GLRenderer->BindVertexArray(0);
		m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER,0);
		m_GLRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		m_GLRenderer->BindTexture(0, false);
	}
};
void ReflexObject::CastShadow()
{

};
_bool ReflexObject::Init(GLRenderer* renderer, EnvMapProbe* Probe)
{
	if (!renderer || !Probe)
		return false;
	this->setRenderer(renderer);
	m_Probe = Probe;
	_float vertices[12] = {-10.0f,0.0f,-10.0f,
							10.0f,0.0f,-10.0f,
							10.0f,0.0f,10.0f,
							-10.0f,0.0f,1.0};
	_float normals[12] = {0.0f,1.0f,0.0f,
							0.0f,1.0f,0.0f,
							0.0f,1.0f,0.0f,
							0.0f,1.0f,0.0f};
	_u32b	indices[6] = { 0,1,2,
							0,2,3 };

	m_ShaderProgram = m_GLRenderer->CreateGLProgramFromFile("..//test//ReflexShader.vs","..//test//ReflexShader.fs" );
	if (!m_ShaderProgram)
		return false;
	
	m_GLRenderer->GenBuffers(1, &m_VertexBuffer);
	if (!m_VertexBuffer)
		return false;
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	m_GLRenderer->setBufferData(GL_ARRAY_BUFFER, 12 * sizeof(_float), vertices, GL_STATIC_DRAW);
	m_GLRenderer->GenBuffers(1, &m_NormalBuffer);
	if (!m_NormalBuffer)
		return false;
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_NormalBuffer);
	m_GLRenderer->setBufferData(GL_ARRAY_BUFFER, 12 * sizeof(_float), normals, GL_STATIC_DRAW);

	m_GLRenderer->GenBuffers(1, &m_IndexBuffer);
	if (!m_IndexBuffer)
		return false;
	m_GLRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	m_GLRenderer->setBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(_u32b), indices, GL_STATIC_DRAW);
	
	m_GLRenderer->GenVertexArrays(1, &m_VAO);
	if (!m_VAO)
		return false;
	m_GLRenderer->BindVertexArray(m_VAO);
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER,m_VertexBuffer);
	m_GLRenderer->SetVertexAttribPointer(0, 3);
	m_GLRenderer->EnableVertexAttribArray(0);
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, m_NormalBuffer);
	m_GLRenderer->SetVertexAttribPointer(1, 3);
	m_GLRenderer->EnableVertexAttribArray(1);
	m_GLRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_IndexBuffer);

	m_GLRenderer->BindVertexArray(0);
	m_GLRenderer->BindBuffer(GL_ARRAY_BUFFER, 0);
	m_GLRenderer->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
};
