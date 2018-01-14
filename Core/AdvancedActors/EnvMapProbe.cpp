#include ".//EnvMapProbe.h"




RGP_CORE::EnvMapProbe::EnvMapProbe() :BaseActor(), m_FBO(0), m_TextureCubeMap(0),
										m_Renderer(NULL), m_Scene(NULL), m_isStatic(true),
										m_isInitialized(false)
{
	for (_s8b i = 0; i < 6; ++i)
		m_PerspCameras[i] = NULL;
};
RGP_CORE::EnvMapProbe::EnvMapProbe(Vertex3d Pos):BaseActor(Pos),m_FBO(0), m_TextureCubeMap(),
													m_Renderer(NULL), m_Scene(NULL), m_isStatic(true),
													m_isInitialized(false)
{
	for (_s8b i = 0; i < 6; ++i)
		m_PerspCameras[i] = NULL;
};

RGP_CORE::EnvMapProbe::~EnvMapProbe() 
{
	this->Destroy();
};
void			RGP_CORE::EnvMapProbe::Destroy() 
{
	for (_s8b i = 0; i < 6; ++i)
		if (m_PerspCameras[i]) {
			delete m_PerspCameras[i];
			m_PerspCameras[i] = NULL;
		}
	if (m_FBO) {
		m_Renderer->DeleteFrameBuffers(1, &m_FBO);
		m_FBO = 0;
	}
	if (m_TextureCubeMap) {
		m_Renderer->DeleteTextures (1, &m_TextureCubeMap);
		m_TextureCubeMap = 0;
	}
	m_Renderer = NULL;
	m_Scene = NULL;
	m_isInitialized = false;
};
_bool			RGP_CORE::EnvMapProbe::Init(GLRenderer* Renderer, GameScene* Scene) 
{
	if (!Renderer)
		return false;
	m_Renderer = Renderer;
	m_Scene = Scene;
	m_Renderer->GenFrameBuffers(1, &m_FBO);
	if (!m_FBO)
		return false;
	m_Renderer->GenTexturesCube(1, &m_TextureCubeMap);
	if (!m_TextureCubeMap)
		return false;

	for (_s8b i = 0; i < 6; ++i) {
		m_PerspCameras[i] = new PerspCamera(M_PI_2, 1.0f, 1.0f, 500.0f);
		if (!m_PerspCameras[i])
			return false;
		m_PerspCameras[i]->setPosition(this->getPosition());
	}

	m_PerspCameras[0]->setOrientation({ 1.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f });//+X
	m_PerspCameras[1]->setOrientation({ -1.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f });//-X
	m_PerspCameras[2]->setOrientation({ 0.0f,1.0f,0.0f }, { 0.0f,0.0f,1.0f });//+Y
	m_PerspCameras[3]->setOrientation({ 0.0f,-1.0f,0.0f }, { 0.0f,0.0f,-1.0f });//-Y
	m_PerspCameras[4]->setOrientation({ 0.0f,0.0f,1.0f }, { 0.0f,1.0f,0.0f });//+Z
	m_PerspCameras[5]->setOrientation({ 0.0f,0.0f,-1.0f }, { 0.0f,1.0f,0.0f });//-Z

	m_isInitialized = true;
	return true;
};
void		RGP_CORE::EnvMapProbe::GenerateEnvMap()
{
	if(m_isInitialized)
	for (_u32b i = 0; i < 6; ++i) {
		m_Renderer->BindFrameBuffer(m_FBO);
		m_Renderer->AttachTexturetoFrameBuffer(GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, m_TextureCubeMap, 0);
		m_Renderer->RenderSceneColors(m_FBO, m_PerspCameras[i]);
	}
	m_Renderer->BindFrameBuffer(0);
};
_bool			RGP_CORE::EnvMapProbe::isStatic() 
{
	return m_isStatic;
};
void			RGP_CORE::EnvMapProbe::setStatic(_bool make) 
{
	m_isStatic = make;
};
GLuint			RGP_CORE::EnvMapProbe::getEnvMap() 
{
	return m_TextureCubeMap;
};

void			RGP_CORE::EnvMapProbe::setPosition(Vertex3d Pos)
{
	if (!m_isInitialized)
		return;
	BaseActor::setPosition(Pos);
	for (_s8b i = 0; i < 6; ++i) {
		if (m_PerspCameras[i])
			m_PerspCameras[i]->setPosition(this->getPosition());
	}
};

