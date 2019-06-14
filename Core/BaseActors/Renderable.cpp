#include ".//Renderable.h"


RGP_CORE::Renderable::Renderable(const _s8b* name):BaseActor(name),m_Visible(true),m_GLRenderer(NULL),m_DoesCastShadow(false), m_EffectByLight(true){
    m_ID |=RENDERABLE ;
};
RGP_CORE::Renderable::Renderable(Vertex3d Pos, const _s8b* name):BaseActor(Pos, name),
												m_GLRenderer(NULL), m_Visible(true), m_DoesCastShadow(false), m_EffectByLight(true)
{
    m_ID |=RENDERABLE ;
};
RGP_CORE::Renderable::Renderable(Vertex3d Pos,Vertex3d Dir,Vertex3d Up, const _s8b* name):BaseActor(Pos,Dir,Up, name),
                                                                    m_Visible(true),m_GLRenderer(NULL),
																	m_DoesCastShadow(false), m_EffectByLight(true)
{
    m_ID |=RENDERABLE ;
};
RGP_CORE::Renderable::~Renderable(){
    this->Destroy();
};
void RGP_CORE::Renderable::Destroy(){
    m_GLRenderer=NULL ;
	BaseActor::Destroy();
};
void RGP_CORE::Renderable::setRenderer(GLRenderer* renderer){
    m_GLRenderer=renderer ;
};
void RGP_CORE::Renderable::setVisible(_bool visible){
    m_Visible=visible ;
};
_bool RGP_CORE::Renderable::isVisible(){
    return m_Visible ;
};
void RGP_CORE::Renderable::setShadowCast(_bool Value) { m_DoesCastShadow = Value; };
_bool  RGP_CORE::Renderable::doesShadowCast(){ return m_DoesCastShadow; };
void	RGP_CORE::Renderable::setEffectedByLight(_bool value) { m_EffectByLight = value; };
_bool	RGP_CORE::Renderable::isEffectedByLight() { return m_EffectByLight; };