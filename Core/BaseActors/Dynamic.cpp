#include ".//Dynamic.h"


TTB::Dynamic::Dynamic():BaseActor(){
	m_ID |=DYNAMIC ;
};
TTB::Dynamic::Dynamic(Vertex3d Pos):BaseActor(Pos){
	m_ID |=DYNAMIC ;
};
TTB::Dynamic::Dynamic(Vertex3d Pos, Vertex3d Dir, Vertex3d Up):BaseActor(Pos,Dir,Up){
	m_ID |=DYNAMIC ;
};
TTB::Dynamic::~Dynamic(){
	this->Destroy();
};
