#include ".//Dynamic.h"


RGP_CORE::Dynamic::Dynamic(const _s8b* name):BaseActor(name){
	m_ID |=DYNAMIC ;
};
RGP_CORE::Dynamic::Dynamic(Vertex3d Pos, const _s8b* name):BaseActor(Pos,name){
	m_ID |=DYNAMIC ;
};
RGP_CORE::Dynamic::Dynamic(Vertex3d Pos, Vertex3d Dir, Vertex3d Up, const _s8b* name):BaseActor(Pos,Dir,Up,name){
	m_ID |=DYNAMIC ;
};
RGP_CORE::Dynamic::~Dynamic(){
	this->Destroy();
};
