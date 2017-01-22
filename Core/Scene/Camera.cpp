#include "Camera.h"
#include <stdio.h>

TTB::Camera::Camera(): m_Near (1.0f),m_Far(1000.0f),
                        m_Pos({0.0f,0.0f,0.0f}),
						m_Dir ({0.0f,0.0f,-1.0f}),
						m_Up({0.0f,1.0f,0.0f}),
						m_ViewMatrix(NULL),m_ProjectionMtx(NULL) {
	m_ViewMatrix=(_float*)malloc(16*sizeof(_float));
	m_ProjectionMtx=(_float*)malloc(16*sizeof(_float));
};
TTB::Camera::~Camera(){
	if(m_ViewMatrix){
		free(m_ViewMatrix);
		m_ViewMatrix=NULL ;
	}
	if(m_ProjectionMtx){
		free(m_ProjectionMtx);
		m_ProjectionMtx=NULL ;
	}
};
void TTB::Camera::setPosition(Vertex3d Pos){
	m_Pos=Pos ;
};
bool TTB::Camera::setOrientation(Vertex3d Dir,Vertex3d Up){
	if(DotProduct3d(Dir,Up)==0.0f){
		m_Dir=Dir;
		m_Up=Up;
		return true ;
	}
	return false ;
};
Vertex3d TTB::Camera::getPosition(){
	return m_Pos;
};
Vertex3d TTB::Camera::getDirection(){
	return m_Dir;
};
Vertex3d TTB::Camera::getUp(){
	return m_Up;
};
Vertex3d TTB::Camera::getLeft(){
    return Normalize3d(CrossProduct3d(m_Up,m_Dir));
}
_float TTB::Camera::getNearValue(){ return m_Near ;};
_float TTB::Camera::getFarValue(){ return m_Far ;};

void TTB::Camera::RotateViaDirection (_float Angle){
    m_Up=Rotate3d(m_Up,m_Dir,Angle);
    if(Magnitude3d(m_Up)!= 1.0f)
        m_Up        =Normalize3d(m_Up);
};
void TTB::Camera::RotateViaUp        (_float Angle){

    m_Dir=Rotate3d(m_Dir,m_Up,Angle);
    if(Magnitude3d(m_Dir)!= 1.0f)
        m_Dir        =Normalize3d(m_Dir);
};
void TTB::Camera::RotateViaLeft      (_float Angle){
    Vertex3d    Left        =Normalize3d(CrossProduct3d(m_Up,m_Dir));
    if(Magnitude3d(Left)!= 1.0f)
        Left        =Normalize3d(Left);
    m_Dir =Normalize3d(Rotate3d(m_Dir,Left,Angle));
    if(Magnitude3d(m_Dir)!= 1.0f)
        m_Dir        =Normalize3d(m_Dir);
    m_Up        =Normalize3d(CrossProduct3d(m_Dir,Left));
    if(Magnitude3d(m_Up)!= 1.0f)
        m_Up        =Normalize3d(m_Up);
};
void TTB::Camera::Translate (Vertex3d ver){
    m_Pos=AddVertex3d(m_Pos,ver);
};
_float* TTB::Camera::getViewMtx(){
	this->UpdateViewMtx();
	return m_ViewMatrix;
};
_float* TTB::Camera::getProjectionMtx(){
    return m_ProjectionMtx ;
};


void TTB::Camera::UpdateViewMtx(){
    Vertex3d f=(Normalize3d(SubsVertex3d(m_Dir , m_Pos)));
	Vertex3d s=(Normalize3d(CrossProduct3d(f, m_Up)));
	Vertex3d u=(CrossProduct3d(s, f));
	m_ViewMatrix[0]=s.x     ; m_ViewMatrix[4]=u.x   ; m_ViewMatrix[8]=  -f.x    ; m_ViewMatrix[12]=-DotProduct3d(s,m_Pos);
    m_ViewMatrix[1]=s.y     ; m_ViewMatrix[5]=u.y   ; m_ViewMatrix[9]=  -f.y    ; m_ViewMatrix[13]=-DotProduct3d(u,m_Pos);
    m_ViewMatrix[2]=s.z     ; m_ViewMatrix[6]=u.z   ; m_ViewMatrix[10]= -f.z    ; m_ViewMatrix[14]=DotProduct3d(f,m_Pos);
    m_ViewMatrix[3]=0.0f    ; m_ViewMatrix[7]=0.0f  ; m_ViewMatrix[11]=0.0f     ; m_ViewMatrix[15]=1.0f   ;
};


TTB::PerspCamera::PerspCamera(_float FOV, _float Aspect, _float Near, _float Far):Camera(){
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
void TTB::PerspCamera::setParameters(_float FOV, _float Aspect, _float Near, _float Far){
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
_float TTB::PerspCamera::getAspectValue(){ return m_Aspect ;};
_float TTB::PerspCamera::getFOVValue(){ return m_FOV; };

void TTB::PerspCamera::UpdateProjectionMtx(){
    float tanHalfFovy = tan(m_FOV / 2.0f);

    m_ProjectionMtx[1]=m_ProjectionMtx[2]=m_ProjectionMtx[3]=0.0f;
    m_ProjectionMtx[4]=m_ProjectionMtx[6]=m_ProjectionMtx[7]=0.0f;
    m_ProjectionMtx[8]=m_ProjectionMtx[9]=0.0f;
    m_ProjectionMtx[12]=m_ProjectionMtx[13]=m_ProjectionMtx[15]=0.0f;

    m_ProjectionMtx[0] = 1.0f / (m_Aspect * tanHalfFovy);
    m_ProjectionMtx[5] = 1.0f / (tanHalfFovy);
    m_ProjectionMtx[11] = -1;// -1 for right handed and 1 for left handed

    if (m_Far-m_Near<=1.0f){
        m_ProjectionMtx[10] = m_Far / (m_Far - m_Near);
        m_ProjectionMtx[14] = -(m_Far * m_Near) / (m_Far - m_Near);
 	}else{
        m_ProjectionMtx[10] = -(m_Far + m_Near) / (m_Far - m_Near);
        m_ProjectionMtx[14] = -(2.0f * m_Far * m_Near) / (m_Far - m_Near);
    }
};



TTB::OrthoCamera::OrthoCamera(_float Left, _float Right, _float Bottom, _float Top, _float Near, _float Far){
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
void TTB::OrthoCamera::setParameters(_float Left, _float Right, _float Bottom, _float Top, _float Near, _float Far){
    if(Left < Right){
        m_Left=Left;
        m_Right=Right ;
    }
    if(Top < Bottom){
        m_Top=Top;
        m_Bottom=Bottom ;
    }
};
_float TTB::OrthoCamera::getLeftValue(){ return m_Left ;};
_float TTB::OrthoCamera::getRightValue(){ return m_Right ;};
_float TTB::OrthoCamera::getBottomValue(){ return m_Bottom ;};
_float TTB::OrthoCamera::getTopValue(){ return m_Top ; };
void TTB::OrthoCamera::UpdateProjectionMtx(){

};

