#include "PhysicalExample.h"

using namespace RGP_CORE;

PModel*	PModel::CreateCube(GLRenderer* renderer,Vertex3d Pos, Vertex3d Dir, Vertex3d Up)
{
	PModel* Object = NULL;
	Object = new PModel();
	if (!Object) {
		printf("allocatio error \n");
		return NULL;
	}
	Object->setRenderer(renderer);
	if (!Object->LoadModelFromFile("..//test//Samples//Cube.obj")) {
		printf("error loading Cubre\n");
		delete Object;
		return NULL;
	}
	/*Object->BaseActor::setPosition(Pos);
	Object->BaseActor::setOrientation(Dir, Up);
	//
	Object->m_Mass = 2.0f;

	btTransform Transfom;
	btVector3 haftextent(2.0, 2.0, 2.0), LocalInertia(0.0, 0.0, 0.0);
	Object->m_Collider = new btCompoundShape();

	Transfom.setFromOpenGLMatrix(Object->getTransMtx());
	btMotionState* motionstate = new btDefaultMotionState(Transfom);


	btCollisionShape* shape = NULL;
	Transfom.setIdentity();
	shape = new btBoxShape(haftextent);
	dynamic_cast<btCompoundShape*>(Object->m_Collider)->addChildShape(Transfom, shape);
	Object->m_Collider->calculateLocalInertia(Object->m_Mass, LocalInertia);
	btRigidBody::btRigidBodyConstructionInfo info(Object->m_Mass, motionstate, Object->m_Collider, LocalInertia);
	Object->m_Rigidbody = new btRigidBody(info);*/

	btVector3 haftextent(2.0, 2.0, 2.0);
	btCollisionShape* shape = NULL;
	btTransform Transform;
	Transform.setIdentity();
	shape = new btBoxShape(haftextent);
	Object->AddCollider(shape,Transform);

	Object->setPosition(Pos);
	Object->setOrientation(Dir, Up);

	return Object;
};
PModel*	PModel::CreateSphere(GLRenderer* renderer, Vertex3d Pos, Vertex3d Dir, Vertex3d Up)
{
	PModel* Object = NULL;
	Object = new PModel();
	if (!Object) {
		printf("allocatio error \n");
		return NULL;
	}
	Object->setRenderer(renderer);
	if (!Object->LoadModelFromFile("..//test//Samples//Sphere.obj")) {
		printf("error loading sphere\n");
		delete Object;
		return NULL;
	}
	/*Object->BaseActor::setPosition(Pos);
	Object->BaseActor::setOrientation(Dir, Up);
	//
	Object->m_Mass = 2.0f;

	btTransform Transfom;
	btVector3  LocalInertia(0.0, 0.0, 0.0);
	Object->m_Collider = new btCompoundShape();
	
	Transfom.setFromOpenGLMatrix(Object->getTransMtx());
	btMotionState* motionstate = new btDefaultMotionState(Transfom);
	
	btCollisionShape* shape = NULL;
	Transfom.setIdentity();
	shape = new btSphereShape(2.0);
	dynamic_cast<btCompoundShape*>(Object->m_Collider)->addChildShape(Transfom, shape);
	Object->m_Collider->calculateLocalInertia(Object->m_Mass, LocalInertia);
	btRigidBody::btRigidBodyConstructionInfo info(Object->m_Mass, motionstate, Object->m_Collider, LocalInertia);
	Object->m_Rigidbody = new btRigidBody(info);*/
	btCollisionShape* shape = NULL;
	btTransform Transform;
	Transform.setIdentity();
	shape = new btSphereShape(2.0);
	Object->AddCollider(shape, Transform);

	Object->setPosition(Pos);
	Object->setOrientation(Dir, Up);

	return Object;
};
PModel*	PModel::CreateGround(GLRenderer* renderer, Vertex3d Pos)
{
	PModel* Object = NULL;
	Object = new PModel();
	if (!Object) {
		printf("allocatio error \n");
		return NULL;
	}
	Object->setRenderer(renderer);
	if (!Object->LoadModelFromFile("..//test//Samples//Ground1.obj")) {
		printf("error loading Ground\n");
		delete Object;
		return NULL;
	}
	/*Object->BaseActor::setPosition(Pos);
	Object->BaseActor::RotateViaDirection(0.5);
	//
	Object->m_Mass = 0.0f;

	btTransform Transfom;
	btVector3 haftextent(15.0, 1.0, 15.0), LocalInertia(0.0, 0.0, 0.0);
	Object->m_Collider = new btCompoundShape();
	
	Transfom.setFromOpenGLMatrix(Object->getTransMtx());
	btMotionState* motionstate = new btDefaultMotionState(Transfom);
	
	
	btCollisionShape* shape = NULL;
	Transfom.setIdentity();
	shape = new btBoxShape(haftextent);
	dynamic_cast<btCompoundShape*>(Object->m_Collider)->addChildShape(Transfom, shape);
	Object->m_Collider->calculateLocalInertia(Object->m_Mass, LocalInertia);
	btRigidBody::btRigidBodyConstructionInfo info(Object->m_Mass, motionstate, Object->m_Collider, LocalInertia);
	Object->m_Rigidbody = new btRigidBody(info);*/
	
	Object->setPosition(Pos);
	Object->m_Mass = 0.0;
	btVector3 haftextent(15.0, 1.0, 15.0);
	btCollisionShape* shape = NULL;
	btTransform Transform;
	Transform.setIdentity();
	shape = new btBoxShape(haftextent);
	Object->AddCollider(shape, Transform);

	
	
	return Object;

};

PModel::PModel() : Model3D(), Physical()
{
};
PModel::~PModel()
{
	this->Destroy();
};
void PModel::Destroy()
{
	Model3D::Destroy();
	Physical::Destroy();
};