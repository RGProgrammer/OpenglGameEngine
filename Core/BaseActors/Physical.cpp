#include ".//Physical.h"


RGP_CORE::Physical::Physical():Dynamic(),m_nbCollisionShapes(NULL),m_CollisionShapes(NULL), m_Collider(NULL),
m_Rigidbody(NULL), m_SoftBody(NULL), m_Velocity({0.0f,0.0f,0.0f}), m_Mass(20.0f)
{
	m_ID |= PHYSICAL ;
	m_Collider = new btCompoundShape();
	btScalar mass=m_Mass;
	btVector3 localInertia(0, 0, 0);
	btTransform transform;
	UpdateTransMtx();
	transform.setFromOpenGLMatrix(m_TransMtx);
	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = NULL;
	myMotionState = new btDefaultMotionState(transform);
	if (!myMotionState)
		printf("no MotionState initialized\n");
	m_Collider = new btCompoundShape();
	if (!m_Collider)
		printf("no Collider adde to the object \n");
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, m_Collider, localInertia);
	m_Rigidbody = new btRigidBody(rbInfo);
	if (!m_Rigidbody)
		printf("Can't create a rigidbody \n");
};
RGP_CORE::Physical::Physical(Vertex3d Pos):Dynamic(Pos), m_nbCollisionShapes(NULL), m_CollisionShapes(NULL), m_Collider(NULL),
m_Rigidbody(NULL), m_SoftBody(NULL), m_Velocity({ 0.0f,0.0f,0.0f }), m_Mass(20.0f)
{
	m_ID |= PHYSICAL ;
	m_Collider = new btCompoundShape();
	btScalar mass=m_Mass;
	btVector3 localInertia(0, 0, 0);
	btTransform transform;
	UpdateTransMtx();
	transform.setFromOpenGLMatrix(m_TransMtx);
	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = NULL;
	myMotionState = new btDefaultMotionState(transform);
	if (!myMotionState)
		printf("no MotionState initialized");
	m_Collider = new btCompoundShape();
	if (!m_Collider)
		printf("no Collider adde to the object \n");
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, m_Collider, localInertia);
	m_Rigidbody = new btRigidBody(rbInfo);
	if (!m_Rigidbody)
		printf("Can't create a rigidbody \n");
};
RGP_CORE::Physical::Physical(Vertex3d Pos, Vertex3d Dir, Vertex3d Up):Dynamic(Pos,Dir,Up), m_nbCollisionShapes(NULL), m_CollisionShapes(NULL), m_Collider(NULL),
m_Rigidbody(NULL), m_SoftBody(NULL), m_Velocity({ 0.0f,0.0f,0.0f }), m_Mass(20.0f)
{
	m_ID |= PHYSICAL ;
	m_Collider = new btCompoundShape();
	btScalar mass=m_Mass;
	btVector3 localInertia(0, 0, 0);
	btTransform transform;
	UpdateTransMtx();
	transform.setFromOpenGLMatrix(m_TransMtx);
	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = NULL;
	myMotionState = new btDefaultMotionState(transform);
	if (!myMotionState)
		printf("no MotionState initialized");
	m_Collider = new btCompoundShape();
	if (!m_Collider)
		printf("no Collider adde to the object \n");
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, m_Collider, localInertia);
	m_Rigidbody = new btRigidBody(rbInfo);
	if (!m_Rigidbody)
		printf("Can't create a rigidbody \n");

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
	if (m_SoftBody) {
		delete m_SoftBody;
		m_SoftBody = NULL;
	};
	m_Velocity = { 0.0f,0.0f,0.0f };
	m_Mass=0.0f;
}
void RGP_CORE::Physical::Update(_float DeltaTime){
	btScalar Ogltransmtx[16];
	btTransform transmtx;
	//m_Position=AddVertex3d(m_Position,m_Velocity);
	//m_Rigidbody->getWorldTransform().getOpenGLMatrix(transmtx);
	btMotionState* motionstate = NULL;
	motionstate = m_Rigidbody->getMotionState();
	if (motionstate) {
		motionstate->getWorldTransform(transmtx);
		transmtx.getOpenGLMatrix(Ogltransmtx);
		m_Position = { Ogltransmtx[12],Ogltransmtx[13],Ogltransmtx[14] };
		m_Direction = { Ogltransmtx[8],Ogltransmtx[9],Ogltransmtx[10] };
		m_Up = { Ogltransmtx[4],Ogltransmtx[5],Ogltransmtx[6] };
	}
	
};
void RGP_CORE::Physical::ApplyTranslationForce(Vertex3d Force){
	//m_AppliedTranslationForce=Force ;
};
void RGP_CORE::Physical::ApplyRotationForce(Vertex3d Force){
	//m_AppliedRotationForce=Force ;

};
_u16b RGP_CORE::Physical::AddCollider(btCollisionShape* collider,const btTransform& transform){
	if(collider){
		btCollisionShape** temp=(btCollisionShape**)malloc((m_nbCollisionShapes+1)*sizeof(btCollisionShape*));
		if(!temp)
			return 0 ;
		for(_u16b i=0 ; i<m_nbCollisionShapes;++i)
			temp[i]=m_CollisionShapes[i];
		temp[m_nbCollisionShapes]=collider ;
		++m_nbCollisionShapes;
		//add the new shape to the compound colliderobject
		m_Collider->addChildShape(transform, collider);
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
btSoftBody*	RGP_CORE::Physical::getSoftBody()
{
	return m_SoftBody;
};
void	RGP_CORE::Physical::setPosition(Vertex3d Pos)
{
	btTransform transform;
	BaseActor::setPosition(Pos);
	transform.setFromOpenGLMatrix(this->getTransMtx());
	m_Rigidbody->setWorldTransform(transform);
};
_bool	RGP_CORE::Physical::setOrientation(Vertex3d Dir, Vertex3d Up)
{
	btTransform transform;
	if (BaseActor::setOrientation(Dir, Up)) {
		transform.setFromOpenGLMatrix(this->getTransMtx());
		m_Rigidbody->setWorldTransform(transform);
		return true;
	}
	return false;
};