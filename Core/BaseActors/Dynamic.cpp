#include ".//Dynamic.h"


RGP_CORE::Dynamic::Dynamic():BaseActor(){
	m_ID |=DYNAMIC ;
};
RGP_CORE::Dynamic::Dynamic(Vertex3d Pos):BaseActor(Pos){
	m_ID |=DYNAMIC ;
};
RGP_CORE::Dynamic::Dynamic(Vertex3d Pos, Vertex3d Dir, Vertex3d Up):BaseActor(Pos,Dir,Up){
	m_ID |=DYNAMIC ;
};
RGP_CORE::Dynamic::~Dynamic(){
	this->Destroy();
};
