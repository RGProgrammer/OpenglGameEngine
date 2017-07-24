#include "PhysicalExample.h"

using namespace RGP_CORE;

PModel*	PModel::CreateCube(GLRenderer* renderer,Vertex3d Pos, Vertex3d Dir, Vertex3d Up)
{
	PModel* Object = NULL;
	Object = new PModel(Pos,Dir,Up);
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
	Object->setPosition(Pos);
	Object->setOrientation(Dir, Up);
	btCollisionShape* shape=NULL ;
	btTransform localTransfom;
	//more code here
	btVector3 haftextent(2.0, 2.0, 2.0);
	shape = new btBoxShape(haftextent);
	localTransfom.setIdentity();
	//
	Object->AddCollider(shape, localTransfom);
	return Object;
};
PModel*	PModel::CreateSphere(GLRenderer* renderer, Vertex3d Pos, Vertex3d Dir, Vertex3d Up)
{
	PModel* Object = NULL;
	Object = new PModel(Pos, Dir, Up);
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
	Object->setPosition(Pos);
	Object->setOrientation(Dir, Up);
	btCollisionShape* shape = NULL;
	btTransform localTransfom;
	//more code here
	shape = new btSphereShape(2.0);
	localTransfom.setIdentity();
	//
	Object->AddCollider(shape, localTransfom);
	return Object;
};
/*PModel*	PModel::CreateCapsule()
{
	PModel* Object = NULL;
	Object = new PModel();
	if (!Object)
		return NULL;
	Object->LoadModelFromFile("..//test//Samples//Capsule.obj");
	btCollisionShape* shape = NULL;
	btTransform localTransfom;
	//more code here
	shape = new btCapsuleShape;
	localTransfom.setIdentity();
	//
	Object->AddCollider(shape, localTransfom);
	return Object;
};*/

PModel::PModel(Vertex3d Pos, Vertex3d Dir, Vertex3d Up) : Model3D(), Physical(Pos,Dir,Up)
{
	printf("type ID : %u\n", m_ID);
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