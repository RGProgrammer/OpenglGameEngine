#include "Camera.h"
#include <stdio.h>

RGP_CORE::Camera::Camera() :BaseActor(),
						m_Near(1.0f), m_Far(1000.0f),
						m_ViewMatrix(NULL),m_ProjectionMtx(NULL),m_Changed(true) {
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
_float RGP_CORE::Camera::getNearValue() { return m_Near; };
_float RGP_CORE::Camera::getFarValue() { return m_Far; };
_float* RGP_CORE::Camera::getViewMtx(){
	return m_ViewMatrix;
};
_float* RGP_CORE::Camera::getProjectionMtx(){
    return m_ProjectionMtx ;
}
_bool RGP_CORE::Camera::hasChanged()
{
	return m_Changed;
}
void RGP_CORE::Camera::ApplyChanges()
{
	this->UpdateViewMtx();
	this->UpdateProjectionMtx();
	m_Changed = false;
}
void RGP_CORE::Camera::setPosition(Vertex3d Pos)
{
	BaseActor::setPosition(Pos);
	m_Changed= true;
}
bool RGP_CORE::Camera::setOrientation(Vertex3d Dir, Vertex3d Up)
{
	_bool ret = BaseActor::setOrientation(Dir,Up);
	if (ret) {
		m_Changed = true;
	}
	return ret;
}
void RGP_CORE::Camera::RotateViaDirection(_float Angle)
{
	BaseActor::RotateViaDirection(Angle);
	m_Changed = true;
}
void RGP_CORE::Camera::RotateViaUp(_float Angle)
{
	BaseActor::RotateViaUp(Angle);
	m_Changed = true;
}
void RGP_CORE::Camera::RotateViaSide(_float Angle)
{
	BaseActor::RotateViaSide(Angle);
	m_Changed = true;
}
void RGP_CORE::Camera::Translate(Vertex3d ver)
{
	BaseActor::Translate(ver);
	m_Changed = true;
}
void RGP_CORE::Camera::TranslateViaSide(_float value)
{
	BaseActor::TranslateViaSide(value);
	m_Changed = true;
}
void RGP_CORE::Camera::TranslateViaUp(_float value)
{
	BaseActor::TranslateViaUp(value);
	m_Changed = true;
}
void RGP_CORE::Camera::TranslateViaDirection(_float value)
{
	BaseActor::TranslateViaDirection(value);
	m_Changed = true;
}



void RGP_CORE::Camera::UpdateViewMtx(){

	FillViewMatrix(m_Position, m_Direction, m_Up, m_ViewMatrix);
};


RGP_CORE::PerspCamera::PerspCamera(): PerspCamera(M_PI_2,1.0f,1.0f,500.0f)
{
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
	FillPersPectiveMatrix(m_FOV, m_Aspect, m_Near, m_Far, m_ProjectionMtx);
};



RGP_CORE::OrthoCamera::OrthoCamera(_float Left, _float Top, _float Right, _float Bottom, _float Near, _float Far){
 
        m_Left=Left;
        m_Right=Right ;
 
        m_Top=Top;
        m_Bottom=Bottom ;
		this->UpdateProjectionMtx();
};
void RGP_CORE::OrthoCamera::setParameters(_float Left, _float Top, _float Right, _float Bottom, _float Near, _float Far){
    if(Left < Right){
        m_Left=Left;
        m_Right=Right ;
    }
    if(Top > Bottom){
        m_Top=Top;
        m_Bottom=Bottom ;
    }
	this->UpdateProjectionMtx();
};
_float RGP_CORE::OrthoCamera::getLeftValue(){ return m_Left ;};
_float RGP_CORE::OrthoCamera::getRightValue(){ return m_Right ;};
_float RGP_CORE::OrthoCamera::getBottomValue(){ return m_Bottom ;};
_float RGP_CORE::OrthoCamera::getTopValue(){ return m_Top ; };
void RGP_CORE::OrthoCamera::UpdateProjectionMtx()
{
	FillOrthographicsMatrix(m_Left, m_Top, m_Right, m_Bottom, m_Near, m_Far,m_ProjectionMtx);
};

