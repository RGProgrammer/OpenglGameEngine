#include  "PhysicsEngine.h"


RGP_CORE::Collider::Collider(Physical*	Owner): BaseActor(),m_Owner(Owner),m_ColliderShape(NOSHAPE)
{};
RGP_CORE::Collider::Collider(Physical*	Owner, Vertex3d Pos): BaseActor(Pos),m_Owner(Owner),m_ColliderShape(NOSHAPE)
{};
RGP_CORE::Collider::Collider(Physical*	Owner ,Vertex3d Pos , Vertex3d Dir, Vertex3d Up): BaseActor(Pos,Dir,Up),m_Owner(Owner),m_ColliderShape(NOSHAPE)
{};
RGP_CORE::Collider::~Collider(){
	this->Destroy();
}
void RGP_CORE::Collider::Destroy(){
	m_Owner=NULL ;
}
RGP_CORE::Shape RGP_CORE::Collider::getColliderShape(){ return m_ColliderShape ;};
RGP_CORE::Physical* RGP_CORE::Collider::getOwner(){ return m_Owner ;};



RGP_CORE::BoxCollider::BoxCollider(Physical*	Owner): Collider(Owner)
{
    m_ColliderShape=BOX_COLLIDER;
};
RGP_CORE::BoxCollider::BoxCollider(Physical*   Owner,Vertex3d Pos): Collider(Owner,Pos){
    m_ColliderShape=BOX_COLLIDER;
};
RGP_CORE::BoxCollider::BoxCollider(Physical*   Owner,Vertex3d Pos, Vertex3d Dir,Vertex3d Up): Collider(Owner,Pos,Dir,Up){
    m_ColliderShape=BOX_COLLIDER;
};
void RGP_CORE::BoxCollider::getData(Vertex3d* MinVertex,Vertex3d* MaxVertex){
	if(MinVertex){
		*MinVertex=m_MinVertex ;
	}
	if(MaxVertex){
		*MaxVertex=m_MaxVertex ;
	}
};


RGP_CORE::ShpereCollider::ShpereCollider(Physical*	Owner): Collider(Owner)
{
    m_ColliderShape=SPHERE_COLLIDER;
};
RGP_CORE::ShpereCollider::ShpereCollider(Physical*   Owner,Vertex3d Pos): Collider(Owner,Pos)
{
    m_ColliderShape=SPHERE_COLLIDER;
};
RGP_CORE::ShpereCollider::ShpereCollider(Physical*   Owner,Vertex3d Pos, Vertex3d Dir,Vertex3d Up): Collider(Owner,Pos,Dir,Up)
{
    m_ColliderShape=SPHERE_COLLIDER;
};
void RGP_CORE::ShpereCollider::getData(_float* Radius){
	if (Radius){
		*Radius=m_Radius ;
	}
};


RGP_CORE::CapsuleCollider::CapsuleCollider(Physical*	Owner): Collider(Owner)
{
    m_ColliderShape=CAPSULE_COLLIDER;
};
RGP_CORE::CapsuleCollider::CapsuleCollider(Physical*   Owner,Vertex3d Pos): Collider(Owner,Pos)
{
    m_ColliderShape=CAPSULE_COLLIDER;
};
RGP_CORE::CapsuleCollider::CapsuleCollider(Physical*   Owner,Vertex3d Pos, Vertex3d Dir,Vertex3d Up): Collider(Owner,Pos,Dir,Up)
{
    m_ColliderShape=CAPSULE_COLLIDER;
};
void RGP_CORE::CapsuleCollider::getData(_float* HalfTubeLen , _float* Radius){
	if (Radius){
		*Radius=m_Radius ;
	}
	if(HalfTubeLen){
		*HalfTubeLen=m_HalfTubeLength ;
	}
};


RGP_CORE::PhysicsEngine::PhysicsEngine():m_CurrentScene(NULL),m_Data(NULL),m_nbColumns(0),m_Size(0),m_Active(true)
{};
RGP_CORE::PhysicsEngine::PhysicsEngine(GameScene * Current):m_CurrentScene(Current),m_Data(NULL),m_nbColumns(0),m_Size(0),m_Active(true)
{};
RGP_CORE::PhysicsEngine::~PhysicsEngine(){
	this->Destroy();
};
void RGP_CORE::PhysicsEngine::Destroy() {
	this->FreeCollisionData();
	m_CurrentScene=NULL ;
};
void RGP_CORE::PhysicsEngine::ActivateEngine(_bool Activate){
	m_Active=Activate ;
};
_bool RGP_CORE::PhysicsEngine::isActive(){
	return m_Active ;
};
RGP_CORE::GameScene* RGP_CORE::PhysicsEngine::getCurrentScene(){
	return m_CurrentScene ;
};
void RGP_CORE::PhysicsEngine::setCurrentScene(GameScene* Scene){
	m_CurrentScene= Scene ;
};
void RGP_CORE::PhysicsEngine::SimulatePhysics(/*Delta time*/){
	if(m_Active){

	}
};
void RGP_CORE::PhysicsEngine::FreeCollisionData(){
	if(m_Data){
		free(m_Data);
		m_Data=NULL ;
		m_nbColumns=0;
		m_Size=0 ;
	}
};
_bool RGP_CORE::PhysicsEngine::CollisionCheck(Physical* obj1,Physical* obj2){
	return true;
};
_bool RGP_CORE::PhysicsEngine::CollisionBetween(BoxCollider* Col1 ,BoxCollider* Col2 ,Vertex3d* Center){
	return true;
};
_bool RGP_CORE::PhysicsEngine::CollisionBetween(BoxCollider* Col1 ,ShpereCollider* Col2 ,Vertex3d*){
	return true;
};
_bool RGP_CORE::PhysicsEngine::CollisionBetween(BoxCollider* Col1 ,CapsuleCollider*  Col2 ,Vertex3d*){
	return true;
};
_bool RGP_CORE::PhysicsEngine::CollisionBetween(BoxCollider* Col1 ,RayCollider* Col2 ,Vertex3d*){
	return true;
};
_bool RGP_CORE::PhysicsEngine::CollisionBetween(ShpereCollider* Col1 ,ShpereCollider* Col2 ,Vertex3d*){
	return true;
};
_bool RGP_CORE::PhysicsEngine::CollisionBetween(ShpereCollider* Col1 ,CapsuleCollider* Col2 ,Vertex3d*){
	return true;
};
_bool RGP_CORE::PhysicsEngine::CollisionBetween(ShpereCollider* Col1 ,RayCollider* Col2 ,Vertex3d*){
	return true;
};
_bool RGP_CORE::PhysicsEngine::CollisionBetween(CapsuleCollider* Col1 ,CapsuleCollider* Col2 ,Vertex3d*){
	return true;
};
_bool RGP_CORE::PhysicsEngine::CollisionBetween(CapsuleCollider* Col1 ,RayCollider* Col2 ,Vertex3d*){
	return true;
};
_bool RGP_CORE::PhysicsEngine::CollisionBetween(RayCollider* Col1 ,RayCollider* Col2 ,Vertex3d*){
	return true;
};
