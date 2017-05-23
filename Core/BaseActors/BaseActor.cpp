#include "BaseActor.h"

RGP_CORE::BaseActor::BaseActor():m_ID(UNKNOWN),  m_Position({0.0f,0.0f,0.0f}), m_Direction({0.0f,0.0f,-1.0f}),
                            m_Up({0.0f,1.0f,0.0f}),  m_TransMtx(NULL),m_Scale({1.0f,1.0f,1.0f})
{
    m_TransMtx=(_float*)malloc(16*sizeof(_float));
	UpdateTransMtx();
};
RGP_CORE::BaseActor::BaseActor(Vertex3d Pos):m_ID(UNKNOWN),  m_Position(Pos), m_Direction({0.0f,0.0f,-1.0f}),
                                        m_Up({0.0f,1.0f,0.0f}),  m_TransMtx(NULL),m_Scale({1.0f,1.0f,1.0f})
{
    m_TransMtx=(_float*)malloc(16*sizeof(_float));
	UpdateTransMtx();
};
RGP_CORE::BaseActor::BaseActor(Vertex3d Pos,Vertex3d Dir,Vertex3d Up):m_ID(UNKNOWN),  m_Position(Pos), m_Direction(Dir),
                                                                m_Up(Up),  m_TransMtx(NULL),m_Scale({1.0f,1.0f,1.0f})
{
	m_TransMtx=(_float*)malloc(16*sizeof(_float));
	UpdateTransMtx();
};
RGP_CORE::BaseActor::~BaseActor(){
	this->Destroy();
};
void RGP_CORE::BaseActor::Destroy(){
	if(m_TransMtx){
		free(m_TransMtx);
	}
	m_TransMtx=NULL ;
};
_u32b RGP_CORE::BaseActor::getID(){
    return m_ID;
};
void RGP_CORE::BaseActor::setPosition(Vertex3d Pos){
	m_Position=Pos ;
	this->UpdateTransMtx();
};
Vertex3d RGP_CORE::BaseActor::getPosition(){
	return m_Position ;
};
bool RGP_CORE::BaseActor::setOrientation(Vertex3d Dir, Vertex3d Up){
	m_Direction = Normalize3d(Dir);
	Vertex3d    Side = Normalize3d(CrossProduct3d(Up, m_Direction));
	m_Up = Normalize3d(CrossProduct3d(Side, m_Direction));
	this->UpdateTransMtx();
	return true;
};
Vertex3d RGP_CORE::BaseActor::getDirection(){
	return m_Direction ;
};
Vertex3d RGP_CORE::BaseActor::getUp(){
	return m_Up;
};
Vertex3d RGP_CORE::BaseActor::getSide(){
	return Normalize3d(CrossProduct3d(m_Up, m_Direction));
};
_float* RGP_CORE::BaseActor::getTransMtx(){
	
	return m_TransMtx ;
};
void RGP_CORE::BaseActor::RotateViaDirection (_float Angle){
    m_Up=Rotate3d(m_Up,m_Direction,Angle);
    if(Magnitude3d(m_Up)!= 1.0f)
        m_Up        =Normalize3d(m_Up);
	this->UpdateTransMtx();
};
void RGP_CORE::BaseActor::RotateViaUp        (_float Angle){

    m_Direction=Rotate3d(m_Direction,m_Up,Angle);
    if(Magnitude3d(m_Direction)!= 1.0f)
        m_Direction        =Normalize3d(m_Direction);
    if(Magnitude3d(m_Direction)!= 1.0f)
        m_Direction        =Normalize3d(m_Direction);
	this->UpdateTransMtx();
};
void RGP_CORE::BaseActor::RotateViaSide      (_float Angle){
	Vertex3d Side = CrossProduct3d(m_Direction, m_Up);
	m_Direction = Normalize3d(Rotate3d(m_Direction, Side, Angle));
	if (Magnitude3d(m_Direction) != 1.0f)
		m_Direction = Normalize3d(m_Direction);
	if (Magnitude3d(m_Direction) != 1.0f)
		m_Direction = Normalize3d(m_Direction);
	//m_Up = Normalize3d(CrossProduct3d(Side, m_Direction));
	m_Up=Normalize3d(Rotate3d(m_Up, Side, Angle));
	if (Magnitude3d(m_Up) != 1.0f)
		m_Up = Normalize3d(m_Up);
	if (Magnitude3d(m_Up) != 1.0f)
		m_Up = Normalize3d(m_Up);

	this->UpdateTransMtx();
};
void RGP_CORE::BaseActor::Translate          (Vertex3d ver){
    m_Position=AddVertex3d(m_Position,ver);
	this->UpdateTransMtx();
};
void RGP_CORE::BaseActor::ScaleUniform(_float value) 
{ 
	m_Scale.x=m_Scale.y=m_Scale.z ;
	this->UpdateTransMtx();
};
void RGP_CORE::BaseActor::Scale(Vertex3d scale){
	m_Scale=scale ;
	this->UpdateTransMtx();
};
Vertex3d RGP_CORE::BaseActor::getScale(){ return m_Scale ;};
void RGP_CORE::BaseActor::UpdateTransMtx(){
	m_Direction=Normalize3d(m_Direction);
	Vertex3d Side = Normalize3d(CrossProduct3d(m_Direction,m_Up));
	m_Up = Normalize3d(CrossProduct3d(Side, m_Direction));
    m_TransMtx[0]=m_Scale.x*Side.x; 	m_TransMtx[4]=m_Scale.y*m_Up.x; 	m_TransMtx[8 ]=m_Scale.z*m_Direction.x; 	m_TransMtx[12]=m_Position.x ;
    m_TransMtx[1]=m_Scale.x*Side.y; 	m_TransMtx[5]=m_Scale.y*m_Up.y; 	m_TransMtx[9 ]=m_Scale.z*m_Direction.y; 	m_TransMtx[13]=m_Position.y ;
    m_TransMtx[2]=m_Scale.x*Side.z; 	m_TransMtx[6]=m_Scale.y*m_Up.z; 	m_TransMtx[10]=m_Scale.z*m_Direction.z; 	m_TransMtx[14]=m_Position.z ;
    m_TransMtx[3]=0.0f  ; 				m_TransMtx[7]=0.0f  ;  				m_TransMtx[11]=0.0f        ; 				m_TransMtx[15]=1.0f         ;
};
