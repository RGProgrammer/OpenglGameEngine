#include  "PhysicsEngine.h"


TTB::Collider::Collider(Physical*	Owner): BaseActor(),m_Owner(Owner),m_ColliderShape(NOSHAPE)
{};
TTB::Collider::Collider(Physical*	Owner, Vertex3d Pos): BaseActor(Pos),m_Owner(Owner),m_ColliderShape(NOSHAPE)
{};
TTB::Collider::Collider(Physical*	Owner ,Vertex3d Pos , Vertex3d Dir, Vertex3d Up): BaseActor(Pos,Dir,Up),m_Owner(Owner),m_ColliderShape(NOSHAPE)
{};
TTB::Collider::~Collider(){
	this->Destroy();
}
void TTB::Collider::Destroy(){
	m_Owner=NULL ;
}
TTB::Shape TTB::Collider::getColliderShape(){ return m_ColliderShape ;};
TTB::Physical* TTB::Collider::getOwner(){ return m_Owner ;};



TTB::BoxCollider::BoxCollider(Physical*	Owner): Collider(Owner)
{
    m_ColliderShape=BOX_COLLIDER;
};
TTB::BoxCollider::BoxCollider(Physical*   Owner,Vertex3d Pos): Collider(Owner,Pos){
    m_ColliderShape=BOX_COLLIDER;
};
TTB::BoxCollider::BoxCollider(Physical*   Owner,Vertex3d Pos, Vertex3d Dir,Vertex3d Up): Collider(Owner,Pos,Dir,Up){
    m_ColliderShape=BOX_COLLIDER;
};
void TTB::BoxCollider::getData(Vertex3d* MinVertex,Vertex3d* MaxVertex){
	if(MinVertex){
		*MinVertex=m_MinVertex ;
	}
	if(MaxVertex){
		*MaxVertex=m_MaxVertex ;
	}
};


TTB::ShpereCollider::ShpereCollider(Physical*	Owner): Collider(Owner)
{
    m_ColliderShape=SPHERE_COLLIDER;
};
TTB::ShpereCollider::ShpereCollider(Physical*   Owner,Vertex3d Pos): Collider(Owner,Pos)
{
    m_ColliderShape=SPHERE_COLLIDER;
};
TTB::ShpereCollider::ShpereCollider(Physical*   Owner,Vertex3d Pos, Vertex3d Dir,Vertex3d Up): Collider(Owner,Pos,Dir,Up)
{
    m_ColliderShape=SPHERE_COLLIDER;
};
void TTB::ShpereCollider::getData(_float* Radius){
	if (Radius){
		*Radius=m_Radius ;
	}
};


TTB::CapsuleCollider::CapsuleCollider(Physical*	Owner): Collider(Owner)
{
    m_ColliderShape=CAPSULE_COLLIDER;
};
TTB::CapsuleCollider::CapsuleCollider(Physical*   Owner,Vertex3d Pos): Collider(Owner,Pos)
{
    m_ColliderShape=CAPSULE_COLLIDER;
};
TTB::CapsuleCollider::CapsuleCollider(Physical*   Owner,Vertex3d Pos, Vertex3d Dir,Vertex3d Up): Collider(Owner,Pos,Dir,Up)
{
    m_ColliderShape=CAPSULE_COLLIDER;
};
void TTB::CapsuleCollider::getData(_float* HalfTubeLen , _float* Radius){
	if (Radius){
		*Radius=m_Radius ;
	}
	if(HalfTubeLen){
		*HalfTubeLen=m_HalfTubeLength ;
	}
};


TTB::PhysicsEngine::PhysicsEngine():m_CurrentScene(NULL),m_Data(NULL),m_nbColumns(0),m_Size(0),m_Active(true)
{};
TTB::PhysicsEngine::PhysicsEngine(GameScene * Current):m_CurrentScene(Current),m_Data(NULL),m_nbColumns(0),m_Size(0),m_Active(true)
{};
TTB::PhysicsEngine::~PhysicsEngine(){
	this->Destroy();
};
void TTB::PhysicsEngine::Destroy() {
	this->FreeCollisionData();
	m_CurrentScene=NULL ;
};
void TTB::PhysicsEngine::ActivateEngine(_bool Activate){
	m_Active=Activate ;
};
_bool TTB::PhysicsEngine::isActive(){
	return m_Active ;
};
TTB::GameScene* TTB::PhysicsEngine::getCurrentScene(){
	return m_CurrentScene ;
};
void TTB::PhysicsEngine::setCurrentScene(GameScene* Scene){
	m_CurrentScene= Scene ;
};
void TTB::PhysicsEngine::SimulatePhysics(/*Delta time*/){
	if(m_Active){

	}
};
void TTB::PhysicsEngine::FreeCollisionData(){
	if(m_Data){
		free(m_Data);
		m_Data=NULL ;
		m_nbColumns=0;
		m_Size=0 ;
	}
};
_bool TTB::PhysicsEngine::CollisionCheck(Physical* obj1,Physical* obj2){

};
_bool TTB::PhysicsEngine::CollisionBetween(BoxCollider* Col1 ,BoxCollider* Col2 ,Vertex3d* Center){

};
_bool TTB::PhysicsEngine::CollisionBetween(BoxCollider* Col1 ,ShpereCollider* Col2 ,Vertex3d*){};
_bool TTB::PhysicsEngine::CollisionBetween(BoxCollider* Col1 ,CapsuleCollider*  Col2 ,Vertex3d*){};
_bool TTB::PhysicsEngine::CollisionBetween(BoxCollider* Col1 ,RayCollider* Col2 ,Vertex3d*){};
_bool TTB::PhysicsEngine::CollisionBetween(ShpereCollider* Col1 ,ShpereCollider* Col2 ,Vertex3d*){};
_bool TTB::PhysicsEngine::CollisionBetween(ShpereCollider* Col1 ,CapsuleCollider* Col2 ,Vertex3d*){};
_bool TTB::PhysicsEngine::CollisionBetween(ShpereCollider* Col1 ,RayCollider* Col2 ,Vertex3d*){};
_bool TTB::PhysicsEngine::CollisionBetween(CapsuleCollider* Col1 ,CapsuleCollider* Col2 ,Vertex3d*){};
_bool TTB::PhysicsEngine::CollisionBetween(CapsuleCollider* Col1 ,RayCollider* Col2 ,Vertex3d*){};
_bool TTB::PhysicsEngine::CollisionBetween(RayCollider* Col1 ,RayCollider* Col2 ,Vertex3d*){};
