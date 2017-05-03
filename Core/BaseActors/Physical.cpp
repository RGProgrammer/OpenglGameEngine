#include ".//Physical.h"


RGP_CORE::Physical::Physical():Dynamic(),m_nbColliders(0),m_Colliders(NULL) , m_Velocity({0.0f,0.0f,0.0f}),
							m_Mass(0.0f),m_AppliedTranslationForce({0.0f,0.0f,0.0f}),m_AppliedRotationForce({0.0f,0.0f,0.0f})
{
	m_ID |= PHYSICAL ;
};
RGP_CORE::Physical::Physical(Vertex3d Pos):Dynamic(Pos),m_nbColliders(0),m_Colliders(NULL) , m_Velocity({0.0f,0.0f,0.0f}),
							m_Mass(0.0f),m_AppliedTranslationForce({0.0f,0.0f,0.0f}),m_AppliedRotationForce({0.0f,0.0f,0.0f})
{
	m_ID |= PHYSICAL ;
};
RGP_CORE::Physical::Physical(Vertex3d Pos, Vertex3d Dir, Vertex3d Up):Dynamic(Pos,Dir,Up),m_nbColliders(0),m_Colliders(NULL) , m_Velocity({0.0f,0.0f,0.0f}),
							m_Mass(0.0f),m_AppliedTranslationForce({0.0f,0.0f,0.0f}),m_AppliedRotationForce({0.0f,0.0f,0.0f})
{
	m_ID |= PHYSICAL ;
};
RGP_CORE::Physical::~Physical(){
	this->Destroy();
};
void RGP_CORE::Physical::Destroy(){
		if(m_Colliders){
			for(_u16b i=0; i<m_nbColliders ; ++i){
				m_Colliders[i]->Destroy();
				delete (m_Colliders[i]);
			}
			free(m_Colliders);
			m_Colliders=NULL ;
		}
		m_nbColliders=0 ;
}
void RGP_CORE::Physical::Update(_float DeltaTime){
	///Calculate m_velocity and then update orientation
	///code should be here.......
	///then reCalculate m_Velocity and update Position
	m_Velocity=ScaleVertex3d(m_AppliedTranslationForce,DeltaTime);
	m_Position=AddVertex3d(m_Position,m_Velocity);
};
void RGP_CORE::Physical::ApplyTranslationForce(Vertex3d Force){
	m_AppliedTranslationForce=Force ;
};
void RGP_CORE::Physical::ApplyRotationForce(Vertex3d Force){
	m_AppliedRotationForce=Force ;

};
_u16b RGP_CORE::Physical::AddCollider(Collider* collider){
	if(collider){
		Collider** temp=(Collider**)malloc((m_nbColliders+1)*sizeof(Collider*));
		if(!temp)
			return 0 ;
		for(_u16b i=0 ; i<m_nbColliders ;++i)
			temp[i]=m_Colliders[i];
		temp[m_nbColliders]=collider ;
		++m_nbColliders ;
		return 1 ;
	}else
		return 0 ;
};
_u16b RGP_CORE::Physical::getNbColliders(){
	return m_nbColliders ;
};
const RGP_CORE::Collider*	RGP_CORE::Physical::getColliderByIndex(_u16b Index){
	if(Index>0 && Index<= m_nbColliders){
		return m_Colliders[Index-1] ;
	}else
		return NULL ;
};
