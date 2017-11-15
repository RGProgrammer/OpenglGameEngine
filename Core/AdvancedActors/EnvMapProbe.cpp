#include ".//EnvMapProbe.h"

RGP_CORE::EnvMapProbe::EnvMapProbe():BaseActor(),m_FBO(0) ,m_TextureCubeMap (0),m_Renderer(NULL ), m_Scene(NULL), m_isStatic(true)
{
};
RGP_CORE::EnvMapProbe::EnvMapProbe(Vertex3d Pos):BaseActor(Pos),m_FBO(0), m_TextureCubeMap(), m_Renderer(NULL), m_Scene(NULL), m_isStatic(true)
{
};

RGP_CORE::EnvMapProbe::~EnvMapProbe() 
{
	this->Destroy();
};
void			RGP_CORE::EnvMapProbe::Destroy() 
{
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
};
_bool			RGP_CORE::EnvMapProbe::Init(GLRenderer* Renderer, GameScene* Scene) 
{
	if (!Renderer)
		return false;
	m_Renderer = Renderer;
	m_Scene = Scene;
	m_Renderer->GenFrameBuffers(1, &m_FBO);
};
void		RGP_CORE::EnvMapProbe::GenerateEnvMap()
{
	for (_u32b i = 0; i < 6; ++i) {
		m_Renderer->AttachTexturetoFrameBuffer(GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, m_TextureCubeMap, 0);
		m_Renderer->DrawSceneColors(m_FBO);
	}
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

_bool RGP_CORE::EnvMapProbe::InitStaticAttributs()
{
	return true;
};