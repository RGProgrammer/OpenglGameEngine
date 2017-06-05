#include ".//Renderable.h"


RGP_CORE::Renderable::Renderable():BaseActor(),m_Visible(true),m_GLRenderer(NULL),m_DoesCastShadow(false){
    m_ID |=RENDERABLE ;
};
RGP_CORE::Renderable::Renderable(Vertex3d Pos):BaseActor(Pos),
												m_GLRenderer(NULL), m_Visible(true), m_DoesCastShadow(false)
{
    m_ID |=RENDERABLE ;
};
RGP_CORE::Renderable::Renderable(Vertex3d Pos,Vertex3d Dir,Vertex3d Up):BaseActor(Pos,Dir,Up),
                                                                    m_Visible(true),m_GLRenderer(NULL),
																	m_DoesCastShadow(false)
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