#include "Camera.h"
#include <stdio.h>

RGP_CORE::Camera::Camera() :BaseActor(),
						m_Near(1.0f), m_Far(1000.0f),
						m_ViewMatrix(NULL),m_ProjectionMtx(NULL) {
	m_ViewMatrix=(_float*)malloc(16*sizeof(_float));
	m_ProjectionMtx=(_float*)malloc(16*sizeof(_float));
};
RGP_CORE::Camera::~Camera(){
	if(m_ViewMatrix){
		free(m_ViewMatrix);
		m_ViewMatrix=NULL ;
	}
	if(m_ProjectionMtx){
		free(m_ProjectionMtx);
		m_ProjectionMtx=NULL ;
	}
};
_float* RGP_CORE::Camera::getViewMtx(){
	this->UpdateViewMtx();
	return m_ViewMatrix;
};
_float* RGP_CORE::Camera::getProjectionMtx(){
    return m_ProjectionMtx ;
};


void RGP_CORE::Camera::UpdateViewMtx(){

	FillViewMatrix(m_Position, m_Direction, m_Up, m_ViewMatrix);
};


RGP_CORE::PerspCamera::PerspCamera(_float FOV, _float Aspect, _float Near, _float Far):Camera(){
    if(FOV>0.0f)
        m_FOV=FOV ;
    else
        m_FOV=45.0f;
    if(Aspect>0.0f)
        m_Aspect=Aspect ;
    else
        m_Aspect=1.0f;
    if(Near < Far && Near >= 0.0f){
        m_Near=Near;
        m_Far=Far ;
    }
    UpdateProjectionMtx();
};
void RGP_CORE::PerspCamera::setParameters(_float FOV, _float Aspect, _float Near, _float Far){
    if(FOV>0.0f){
        m_FOV=FOV ;
    }
    if(Aspect>0.0f){
        m_Aspect=Aspect ;
    }
    if(Near < Far && Near >= 0.0f){
        m_Near=Near;
        m_Far=Far ;
    }
    UpdateProjectionMtx();
};
_float RGP_CORE::PerspCamera::getAspectValue(){ return m_Aspect ;};
_float RGP_CORE::PerspCamera::getFOVValue(){ return m_FOV; };

void RGP_CORE::PerspCamera::UpdateProjectionMtx(){
	FillPersPectiveMatrix(m_Aspect, m_FOV, m_Near, m_Far, m_ProjectionMtx);
};



RGP_CORE::OrthoCamera::OrthoCamera(_float Left, _float Right, _float Bottom, _float Top, _float Near, _float Far){
    if(Left < Right){
        m_Left=Left;
        m_Right=Right ;
    }else{
        m_Left=0.0f;
        m_Right=1.0f ;
    }
    if(Top < Bottom){
        m_Top=Top;
        m_Bottom=Bottom ;
    }else{
        m_Top=0.0f;
        m_Bottom=1.0f;
    }
};
void RGP_CORE::OrthoCamera::setParameters(_float Left, _float Right, _float Top, _float Bottom, _float Near, _float Far){
    if(Left < Right){
        m_Left=Left;
        m_Right=Right ;
    }
    if(Top < Bottom){
        m_Top=Top;
        m_Bottom=Bottom ;
    }
};
_float RGP_CORE::OrthoCamera::getLeftValue(){ return m_Left ;};
_float RGP_CORE::OrthoCamera::getRightValue(){ return m_Right ;};
_float RGP_CORE::OrthoCamera::getBottomValue(){ return m_Bottom ;};
_float RGP_CORE::OrthoCamera::getTopValue(){ return m_Top ; };
void RGP_CORE::OrthoCamera::UpdateProjectionMtx()
{
	FillOrthographicsMatrix(m_Left, m_Right, m_Top, m_Bottom, m_Near, m_Far);
};

