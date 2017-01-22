#include "BaseActor.h"

TTB::BaseActor::BaseActor():BaseActor({0.0f,0.0f,0.0f})
{};
TTB::BaseActor::BaseActor(Vertex3d Pos):BaseActor(Pos,{0.0f,0.0f,1.0f},{0.0f,1.0f,0.0f})
{};
TTB::BaseActor::BaseActor(Vertex3d Pos,Vertex3d Dir,Vertex3d Up):m_ID(UNKNOWN),  m_Position(Pos), m_Direction(Dir),
			m_Up(Up),  m_TransMtx(NULL),m_Scale({1.0f,1.0f,1.0f})
{
	m_TransMtx=(_float*)malloc(16*sizeof(_float));
	UpdateTransMtx();
};
TTB::BaseActor::~BaseActor(){
	this->Destroy();
};
void TTB::BaseActor::Destroy(){
	if(m_TransMtx){
		free(m_TransMtx);
	}
	m_TransMtx=NULL ;
};
_u32b TTB::BaseActor::getID(){
    return m_ID;
};
void TTB::BaseActor::setPosition(Vertex3d Pos){
	m_Position=Pos ;
};
Vertex3d TTB::BaseActor::getPosition(){
	return m_Position ;
};
bool TTB::BaseActor::setOrientation(Vertex3d Dir, Vertex3d Up){
	if(DotProduct3d(Dir,Up)==0.0f){
		m_Direction=Dir;
		m_Up=Up ;
		return true ;
	}
	return false ;
};
Vertex3d TTB::BaseActor::getDirection(){
	return m_Direction ;
};
Vertex3d TTB::BaseActor::getUp(){
	return m_Up;
};
Vertex3d TTB::BaseActor::getLeft(){
    return Normalize3d(CrossProduct3d(m_Up,m_Direction));
};
_float* TTB::BaseActor::getTransMtx(){
	UpdateTransMtx();
	return m_TransMtx ;
};
void TTB::BaseActor::RotateViaDirection (_float Angle){
    m_Up=Rotate3d(m_Up,m_Direction,Angle);
    m_Up=Normalize3d(m_Up);
};
void TTB::BaseActor::RotateViaUp        (_float Angle){
    m_Direction=Rotate3d(m_Direction,m_Up,Angle);
    m_Direction=Normalize3d(m_Direction);
};
void TTB::BaseActor::RotateViaLeft      (_float Angle){
    Vertex3d    Left        =Normalize3d(CrossProduct3d(m_Up,m_Direction));
    if(Magnitude3d(Left)!= 1.0f)
        Left        =Normalize3d(Left);
    m_Direction =Normalize3d(Rotate3d(m_Direction,Left,Angle));
    if(Magnitude3d(m_Direction)!= 1.0f)
        m_Direction        =Normalize3d(m_Direction);
    m_Up        =Normalize3d(CrossProduct3d(m_Direction,Left));
    if(Magnitude3d(m_Up)!= 1.0f)
        m_Up        =Normalize3d(m_Up);
};
void TTB::BaseActor::Translate          (Vertex3d ver){
    m_Position=AddVertex3d(m_Position,ver);
};
void TTB::BaseActor::ScaleUniform(_float value) { m_Scale.x=m_Scale.y=m_Scale.z ;};
void TTB::BaseActor::Scale(Vertex3d scale){m_Scale=scale ;};
Vertex3d TTB::BaseActor::getScale(){ return m_Scale ;};
void TTB::BaseActor::UpdateTransMtx(){
	m_Direction=Normalize3d(m_Direction);
	m_Up=Normalize3d(m_Up);
	Vertex3d left=Normalize3d(CrossProduct3d(m_Up,m_Direction));
    m_TransMtx[0]=m_Scale.x*left.x; 	m_TransMtx[4]=m_Scale.y*m_Up.x; 	m_TransMtx[8 ]=m_Scale.z*m_Direction.x; 	m_TransMtx[12]=m_Position.x ;
    m_TransMtx[1]=m_Scale.x*left.y; 	m_TransMtx[5]=m_Scale.y*m_Up.y; 	m_TransMtx[9 ]=m_Scale.z*m_Direction.y; 	m_TransMtx[13]=m_Position.y ;
    m_TransMtx[2]=m_Scale.x*left.z; 	m_TransMtx[6]=m_Scale.y*m_Up.z; 	m_TransMtx[10]=m_Scale.z*m_Direction.z; 	m_TransMtx[14]=m_Position.z ;
    m_TransMtx[3]=0.0f  ; 				m_TransMtx[7]=0.0f  ;  				m_TransMtx[11]=0.0f        ; 				m_TransMtx[15]=1.0f         ;
};
