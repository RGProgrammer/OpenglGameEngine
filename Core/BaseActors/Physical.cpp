#include ".//Physical.h"


RGP_CORE::Physical::Physical():Dynamic(),m_nbCollisionShapes(NULL),m_CollisionShapes(NULL), m_Collider(NULL),
m_Rigidbody(NULL), m_Velocity({0.0f,0.0f,0.0f}), m_Mass(20.0f)//, m_SoftBody(NULL)
{
	m_ID |= PHYSICAL ;
	btVector3 localInertia= btVector3(0.0,0.0,0.0);
	btTransform transform;
	btMotionState* motionstate = NULL;
	
	m_Collider = new btCompoundShape();
	m_Collider->calculateLocalInertia(m_Mass, localInertia);
	transform.setFromOpenGLMatrix(this->getTransMtx());

	motionstate = new btDefaultMotionState(transform);

	btRigidBody::btRigidBodyConstructionInfo info(m_Mass, motionstate, m_Collider, localInertia);

	m_Rigidbody = new btRigidBody(info);
	
};
RGP_CORE::Physical::Physical(Vertex3d Pos):Dynamic(Pos), m_nbCollisionShapes(NULL), m_CollisionShapes(NULL), m_Collider(NULL),
m_Rigidbody(NULL), m_Velocity({ 0.0f,0.0f,0.0f }), m_Mass(20.0f)//, m_SoftBody(NULL)
{
	m_ID |= PHYSICAL ;
	btVector3 localInertia = btVector3(0.0, 0.0, 0.0);
	btTransform transform;
	btMotionState* motionstate = NULL;

	m_Collider = new btCompoundShape();
	m_Collider->calculateLocalInertia(m_Mass, localInertia);
	transform.setFromOpenGLMatrix(this->getTransMtx());

	motionstate = new btDefaultMotionState(transform);

	btRigidBody::btRigidBodyConstructionInfo info(m_Mass, motionstate, m_Collider, localInertia);

	m_Rigidbody = new btRigidBody(info);
};
RGP_CORE::Physical::Physical(Vertex3d Pos, Vertex3d Dir, Vertex3d Up):Dynamic(Pos,Dir,Up), m_nbCollisionShapes(NULL), m_CollisionShapes(NULL), m_Collider(NULL),
m_Rigidbody(NULL), m_Velocity({ 0.0f,0.0f,0.0f }), m_Mass(20.0f)//, m_SoftBody(NULL)
{
	m_ID |= PHYSICAL ;
	btVector3 localInertia = btVector3(0.0, 0.0, 0.0);
	btTransform transform;
	btMotionState* motionstate = NULL;

	m_Collider = new btCompoundShape();
	m_Collider->calculateLocalInertia(m_Mass, localInertia);
	transform.setFromOpenGLMatrix(this->getTransMtx());

	motionstate = new btDefaultMotionState(transform);

	btRigidBody::btRigidBodyConstructionInfo info(m_Mass, motionstate, m_Collider, localInertia);

	m_Rigidbody = new btRigidBody(info);
};
RGP_CORE::Physical::~Physical(){
	this->Destroy();
};
void RGP_CORE::Physical::Destroy(){

	if (m_Rigidbody) {
		if (m_Rigidbody->getMotionState())
			delete m_Rigidbody->getMotionState();
		delete m_Rigidbody;
		m_Rigidbody = NULL;
	}
	if (m_Collider) {
		delete m_Rigidbody;
		m_Rigidbody = NULL;
	}
	if(m_CollisionShapes) {
		for (_u32b i = 0 ; i < m_nbCollisionShapes; ++i)
			delete m_CollisionShapes[i];
		free( m_CollisionShapes);
		m_CollisionShapes = NULL;
	}
	//if (m_SoftBody) {
	//	delete m_SoftBody;
	//	m_SoftBody = NULL;
	//};
	m_Velocity = { 0.0f,0.0f,0.0f };
	m_Mass=0.0f;
}
void RGP_CORE::Physical::Update(_float DeltaTime){
	
	this->SyncActor();
};
void RGP_CORE::Physical::ApplyTranslationForce(Vertex3d Force){
	//m_AppliedTranslationForce=Force ;
};
void RGP_CORE::Physical::ApplyRotationForce(Vertex3d Force){
	//m_AppliedRotationForce=Force ;

};
_u16b RGP_CORE::Physical::AddCollider(btCollisionShape* collider,const btTransform& transform){
	if(collider && m_Collider->getShapeType()==COMPOUND_SHAPE_PROXYTYPE){
		btVector3 inertia;
		btCollisionShape** temp=(btCollisionShape**)malloc((m_nbCollisionShapes+1)*sizeof(btCollisionShape*));
		if(!temp)
			return 0 ;
		for(_u16b i=0 ; i<m_nbCollisionShapes;++i)
			temp[i]=m_CollisionShapes[i];
		temp[m_nbCollisionShapes]=collider ;
		++m_nbCollisionShapes;
		//add the new shape to the compound colliderobject
		dynamic_cast<btCompoundShape*>(m_Collider)->addChildShape(transform, collider);
		printf("Collision shape added\n");
		m_Collider->calculateLocalInertia(this->m_Mass, inertia);
		m_Rigidbody->setMassProps(m_Mass, inertia);
		return 1 ;
	}else
		return 0 ;
};
_u16b RGP_CORE::Physical::getNbColliders(){
	return m_nbCollisionShapes ;
};
btCollisionShape*	RGP_CORE::Physical::getColliderByIndex(_u16b Index){
	if (Index > 0 && Index <= m_nbCollisionShapes) {
		return m_CollisionShapes[Index];
	}
	else
		return NULL ;
};

btRigidBody*	RGP_CORE::Physical::getRigidBody()
{
	return m_Rigidbody;
};
//btSoftBody*	RGP_CORE::Physical::getSoftBody()
//{
//	return m_SoftBody;
//};
void	RGP_CORE::Physical::setPosition(Vertex3d Pos)
{
	
	BaseActor::setPosition(Pos);
	this->SyncPhysics();
	
};
_bool	RGP_CORE::Physical::setOrientation(Vertex3d Dir, Vertex3d Up)
{
	if (BaseActor::setOrientation(Dir, Up)) {
		this->SyncPhysics();
		return true;
	}
	return false;
};

void RGP_CORE::Physical::RotateViaDirection(_float Angle)
{
	BaseActor::RotateViaDirection(Angle);
	this->SyncPhysics();
};
void RGP_CORE::Physical::RotateViaUp(_float Angle) 
{
	BaseActor::RotateViaUp(Angle);
	this->SyncPhysics();

};
void RGP_CORE::Physical::RotateViaSide(_float Angle)
{
	BaseActor::RotateViaSide(Angle);
	this->SyncPhysics();
};

void RGP_CORE::Physical::SyncActor()
{
	btTransform transmtx;
	btScalar	ogMtx[16];
	btMotionState* motionstate = NULL;
	motionstate = m_Rigidbody->getMotionState();
	if (motionstate) {
		motionstate->getWorldTransform(transmtx);
		transmtx.getOpenGLMatrix(ogMtx);
		BaseActor::setPosition({ ogMtx[12],ogMtx[13],ogMtx[14] });
		BaseActor::setOrientation({ ogMtx[8],ogMtx[9],ogMtx[10] }, { ogMtx[4],ogMtx[5],ogMtx[6] });
	}
	else {
		transmtx = m_Rigidbody->getWorldTransform();
		transmtx.getOpenGLMatrix(ogMtx);
		BaseActor::setPosition({ ogMtx[12],ogMtx[13],ogMtx[14] });
		BaseActor::setOrientation({ ogMtx[8],ogMtx[9],ogMtx[10] }, { ogMtx[4],ogMtx[5],ogMtx[6] });
	}
};

void RGP_CORE::Physical::SyncPhysics()
{
	btTransform transform;
	transform.setFromOpenGLMatrix(this->getTransMtx());
	if (m_Rigidbody->getMotionState())
		m_Rigidbody->getMotionState()->setWorldTransform(transform);
	m_Rigidbody->setWorldTransform(transform);
};