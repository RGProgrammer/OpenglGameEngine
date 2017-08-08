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
	Object->BaseActor::setPosition(Pos);
	Object->BaseActor::setOrientation(Dir, Up);
	//
	Object->m_Mass = 2.0f;
	btCollisionShape* shape=NULL ;
	btTransform localTransfom;
	btVector3 haftextent(2.0, 2.0, 2.0),LocalInertia(0.0,0.0,0.0);
	
	shape = new btBoxShape(haftextent);
	localTransfom.setFromOpenGLMatrix(Object->getTransMtx());
	shape->calculateLocalInertia(Object->m_Mass,LocalInertia);
	Object->m_Collider = shape;

	btMotionState* motionstate = new btDefaultMotionState(localTransfom);

	btRigidBody::btRigidBodyConstructionInfo info(Object->m_Mass, motionstate, Object->m_Collider, LocalInertia);
	Object->m_Rigidbody = new btRigidBody(info);
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
	Object->BaseActor::setPosition(Pos);
	Object->BaseActor::setOrientation(Dir, Up);
	
	//
	Object->m_Mass = 2.0f;
	btCollisionShape* shape = NULL;
	btTransform localTransfom;
	btVector3  LocalInertia(0.0, 0.0, 0.0);
	
	shape = new btSphereShape(2.0);
	localTransfom.setFromOpenGLMatrix(Object->getTransMtx());
	shape->calculateLocalInertia(Object->m_Mass, LocalInertia);
	Object->m_Collider = shape;

	btMotionState* motionstate = new btDefaultMotionState(localTransfom);

	btRigidBody::btRigidBodyConstructionInfo info(Object->m_Mass, motionstate, Object->m_Collider, LocalInertia);
	Object->m_Rigidbody = new btRigidBody(info);

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
	if (!Object->LoadModelFromFile("..//test//Samples//Ground.obj")) {
		printf("error loading Ground\n");
		delete Object;
		return NULL;
	}
	Object->BaseActor::setPosition(Pos);
	Object->BaseActor::RotateViaDirection(0.5);
	//
	Object->m_Mass = 0.0f;
	btCollisionShape* shape = NULL;
	btTransform localTransfom;
	btVector3 haftextent(15.0, 1.0, 15.0), LocalInertia(0.0, 0.0, 0.0);
	
	shape = new btBoxShape(haftextent);
	if (!shape)
		printf("error creating RootCollision shape\n");
	localTransfom.setFromOpenGLMatrix(Object->getTransMtx());
	shape->calculateLocalInertia(Object->m_Mass, LocalInertia);
	Object->m_Collider = shape;
	btMotionState* motionstate = new btDefaultMotionState(localTransfom);
	if (!motionstate)
		printf("error creating motion state\n");
	btRigidBody::btRigidBodyConstructionInfo info(Object->m_Mass, motionstate, Object->m_Collider, LocalInertia);
	Object->m_Rigidbody = NULL;
	Object->m_Rigidbody = new btRigidBody(info);
	if (!Object->m_Rigidbody) {
		printf("Cannot create rigidbody \n");
		delete Object;
		Object = NULL;
	}
	
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