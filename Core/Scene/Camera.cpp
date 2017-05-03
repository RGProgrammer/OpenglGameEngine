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
void RGP_CORE::Camera::setPosition(Vertex3d Pos){
	m_Position=Pos ;
};
bool RGP_CORE::Camera::setOrientation(Vertex3d Dir,Vertex3d Up){
	if(DotProduct3d(Dir,Up)==0.0f){
		m_Direction=Dir;
		m_Up=Up;
		return true ;
	}
	return false ;
};
Vertex3d RGP_CORE::Camera::getPosition(){
	return m_Position;
};
Vertex3d RGP_CORE::Camera::getDirection(){
	return m_Direction;
};
Vertex3d RGP_CORE::Camera::getUp(){
	return m_Up;
};
Vertex3d RGP_CORE::Camera::getSide(){
    return Normalize3d(CrossProduct3d(m_Up,m_Direction));
}
_float RGP_CORE::Camera::getNearValue(){ return m_Near ;};
_float RGP_CORE::Camera::getFarValue(){ return m_Far ;};

void RGP_CORE::Camera::RotateViaDirection (_float Angle){
    m_Up=Rotate3d(m_Up,m_Direction,Angle);
    if(Magnitude3d(m_Up)!= 1.0f)
        m_Up        =Normalize3d(m_Up);
};
void RGP_CORE::Camera::RotateViaUp        (_float Angle){

    m_Direction=Rotate3d(m_Direction,m_Up,Angle);
    if(Magnitude3d(m_Direction)!= 1.0f)
        m_Direction        =Normalize3d(m_Direction);
    if(Magnitude3d(m_Direction)!= 1.0f)
        m_Direction        =Normalize3d(m_Direction);
};
void RGP_CORE::Camera::RotateViaSide      (_float Angle){
    Vertex3d    Side        =Normalize3d(CrossProduct3d(m_Direction,m_Up));
    if(Magnitude3d(Side)!= 1.0f)
        Side        =Normalize3d(Side);
    m_Direction =Normalize3d(Rotate3d(m_Direction,Side,Angle));
    if(Magnitude3d(m_Direction)!= 1.0f)
        m_Direction        =Normalize3d(m_Direction);
    m_Up        =Normalize3d(CrossProduct3d(Side,m_Direction));
    if(Magnitude3d(m_Up)!= 1.0f)
        m_Up        =Normalize3d(m_Up);
};
void RGP_CORE::Camera::Translate (Vertex3d ver){
    m_Position.x+=ver.x;
    m_Position.y+=ver.y;
    m_Position.z+=ver.z;
};
_float* RGP_CORE::Camera::getViewMtx(){
	this->UpdateViewMtx();
	return m_ViewMatrix;
};
_float* RGP_CORE::Camera::getProjectionMtx(){
    return m_ProjectionMtx ;
};


void RGP_CORE::Camera::UpdateViewMtx(){
   
    Vertex3d    Side        =Normalize3d(CrossProduct3d(m_Direction,m_Up));
    m_ViewMatrix[0]=Side.x			; m_ViewMatrix[4]=Side.y		 ; m_ViewMatrix[8]=  Side.z				; m_ViewMatrix[12]=-DotProduct3d(Side,m_Position);
    m_ViewMatrix[1]=m_Up.x			; m_ViewMatrix[5]=m_Up.y		 ; m_ViewMatrix[9]=  m_Up.z				; m_ViewMatrix[13]=-DotProduct3d(m_Up,m_Position);
    m_ViewMatrix[2]=-m_Direction.x  ; m_ViewMatrix[6]=-m_Direction.y ; m_ViewMatrix[10]=-m_Direction.z      ; m_ViewMatrix[14]=DotProduct3d(m_Direction,m_Position);
    m_ViewMatrix[3]=0.0f       ; m_ViewMatrix[7]=0.0f     ; m_ViewMatrix[11]= 0.0f       ; m_ViewMatrix[15]=1.0f   ;


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
    float tanHalfFovy = tan(m_FOV / 2.0f);

    m_ProjectionMtx[1]=m_ProjectionMtx[2]=m_ProjectionMtx[3]=0.0f;
    m_ProjectionMtx[4]=m_ProjectionMtx[6]=m_ProjectionMtx[7]=0.0f;
    m_ProjectionMtx[8]=m_ProjectionMtx[9]=0.0f;
    m_ProjectionMtx[12]=m_ProjectionMtx[13]=m_ProjectionMtx[15]=0.0f;

    m_ProjectionMtx[0] = 1.0f / (m_Aspect * tanHalfFovy);
    m_ProjectionMtx[5] = 1.0f / (tanHalfFovy);
    m_ProjectionMtx[11] = -1;// -1 for right handed and 1 for left handed

    //m_ProjectionMtx[10] = m_Far / (m_Far - m_Near);
    //m_ProjectionMtx[14] = -(m_Far * m_Near) / (m_Far - m_Near);

    m_ProjectionMtx[10] = -(m_Far + m_Near) / (m_Far - m_Near);
    m_ProjectionMtx[14] = -(2.0f * m_Far * m_Near) / (m_Far - m_Near);

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
void RGP_CORE::OrthoCamera::setParameters(_float Left, _float Right, _float Bottom, _float Top, _float Near, _float Far){
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
void RGP_CORE::OrthoCamera::UpdateProjectionMtx(){

};

