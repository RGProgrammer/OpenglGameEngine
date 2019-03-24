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
	if (!Object->LoadModelFromFile("..//Resources//Cube.obj")) {
		printf("error loading cube\n");
		delete Object;
		return NULL;
	}
	

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
	if (!Object->LoadModelFromFile("..//Resources//sphere.obj")) {
		printf("error loading sphere\n");
		delete Object;
		return NULL;
	}
	
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
	if (!Object->LoadModelFromFile("..//Resources//Ground1.obj")) {
		printf("error loading Ground\n");
		delete Object;
		return NULL;
	}
	
	
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