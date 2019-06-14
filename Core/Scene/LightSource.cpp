#include ".//LightSource.h"

RGP_CORE::LightSource::LightSource() : LightSource({0.0f,0.0f,0.0f})
{ 
};

RGP_CORE::LightSource::LightSource(Vertex3d Pos) : LightSource(Pos, { 0.0f,0.0f,1.0f }, { 0.0f,1.0f,0.0f })
{ 
};

RGP_CORE::LightSource::LightSource(Vertex3d Pos,Vertex3d Dir,Vertex3d Up): BaseActor(Pos,Dir,Up),
										m_Distance(10.f), m_Shiness(1.0f), m_CutoffAngle(-1.0f),
										m_LightViewMtx(NULL), m_LightProjectionMtx(NULL), m_ShadowStrengh(0.0)
{ 
	m_ID |= LIGHTSOURCE ;
	m_DiffuseColor[0] = 1.0f; m_DiffuseColor[1] = 1.0f; m_DiffuseColor[2] = 1.0f;
	m_SpecularColor[0] = 1.0f; m_SpecularColor[1] = 1.0f; m_SpecularColor[2] = 1.0f;
	m_AmbientColor[0] = 0.2f; m_AmbientColor[1] = 0.2f; m_AmbientColor[2] = 0.2f;
	m_LightProjectionMtx = (_float*)malloc(16 * sizeof(_float));
	m_LightViewMtx = (_float*)malloc(96 * sizeof(_float));//contains all the View matrices for a point light
};

RGP_CORE::LightSource::~LightSource(){
    this->Destroy();
};
void RGP_CORE::LightSource::Destroy() 
{
	BaseActor::Destroy();
	if (m_LightViewMtx) {
		free(m_LightViewMtx);
		m_LightViewMtx = NULL;
	}
	if (m_LightProjectionMtx) {
		free(m_LightProjectionMtx);
		m_LightProjectionMtx = NULL;
	}
};
void RGP_CORE::LightSource::setLightDiffuseColor(_float red, _float green, _float blue){
    float Sum=red+green+blue ;
    m_DiffuseColor[0]=red/Sum ;
    m_DiffuseColor[1]=green/Sum ;
    m_DiffuseColor[2]=blue/Sum ;
};
void RGP_CORE::LightSource::setLightDiffuseColor(Vertex3d color){
    float Sum=color.x+color.y+color.z ;
    m_DiffuseColor[0]=color.x/Sum;
    m_DiffuseColor[1]=color.y/Sum;
    m_DiffuseColor[2]=color.z/Sum;
};
void RGP_CORE::LightSource::setLightSpecularColor(_float red, _float green, _float blue){
    float Sum=red+green+blue ;
    m_SpecularColor[0]=red/Sum ;
    m_SpecularColor[1]=green/Sum ;
    m_SpecularColor[2]=blue/Sum ;
};
void RGP_CORE::LightSource::setLightSpecularColor(Vertex3d color){
    float Sum=color.x+color.y+color.z ;
    m_SpecularColor[0]=color.x/Sum;
    m_SpecularColor[1]=color.y/Sum;
    m_SpecularColor[2]=color.z/Sum;
};
void RGP_CORE::LightSource::setLightAmbientColor(_float red, _float green, _float blue){
    float Sum=red+green+blue ;
    m_AmbientColor[0]=red/Sum ;
    m_AmbientColor[1]=green/Sum ;
    m_AmbientColor[2]=blue/Sum ;
};
void RGP_CORE::LightSource::setLightAmbientColor(Vertex3d color){
    float Sum=color.x+color.y+color.z ;
    m_AmbientColor[0]=color.x/Sum;
    m_AmbientColor[1]=color.y/Sum;
    m_AmbientColor[2]=color.z/Sum;
};
void RGP_CORE::LightSource::setLightDistance(_float Distance){
    m_Distance=Distance ;
};
void RGP_CORE::LightSource::setLightShiness(_float Strength){
    m_Shiness=Strength ;
};
void RGP_CORE::LightSource::setShadowStrengh(_float level)
{
	m_ShadowStrengh = level;
	if (m_ShadowStrengh > 1.0f)
		m_ShadowStrengh = 1.0f;
	else if (m_ShadowStrengh < 0.0)
		m_ShadowStrengh = 0.0f;
};
const float* RGP_CORE::LightSource::getLightDiffuseColor(){ return m_DiffuseColor;};
const float* RGP_CORE::LightSource::getLightSpecularColor(){ return m_SpecularColor;};
const float* RGP_CORE::LightSource::getLightAmbientColor(){return m_AmbientColor ;};
_float   RGP_CORE::LightSource::getLightDistance(){ return m_Distance ;};
_float   RGP_CORE::LightSource::getLightAttinuation(){ return 1.0f / m_Distance ;};
_float   RGP_CORE::LightSource::getLightCutoffAngle(){ return m_CutoffAngle ;};
_float   RGP_CORE::LightSource::getLightShiness(){return m_Shiness ;};
_float   RGP_CORE::LightSource::getShadowStrengh() {return m_ShadowStrengh;};
_float*	RGP_CORE::LightSource::getLightViewMtx(){ 
	UpdateLightViewMtx();
	return m_LightViewMtx; };
_float*	RGP_CORE::LightSource::getLightProjectionMtx(Camera*	selected){
	UpdateLightProjectionMtx(selected);
	return m_LightProjectionMtx; };
